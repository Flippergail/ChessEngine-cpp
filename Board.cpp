#include "Board.h"

Board::Board() {

    // creates board cells
    for (short int y = 0; y < board_size; ++y) {
        for (short int x = 0; x < board_size; x++) {
            board[x][y].coord = Coord{ x, y };
        }
    }

    // creates main pieces

    for (short int i = 0; i < 2; i++)
    {
        //Piece r1(r, i); Piece n1(n, i); Piece b1(b, i); Piece q1(q, i); Piece k1(k, i); Piece b2(b, i); Piece n2(n, i); Piece r2(r, i);
        short int p_y{ i * 7 };
        //board[0][p_y] = &r1; board[1][p_y] = &n1; board[2][p_y] = &b1; board[3][p_y] = &q1; board[4][p_y] = &k1; board[5][p_y] = &b2; board[6][p_y] = &n2; board[7][p_y] = &r2;
        Coord init_piece_coord{ 0, p_y };
        board[0][p_y].piece = new Piece(r, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[1][p_y].piece = new Piece(n, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[2][p_y].piece = new Piece(b, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[3][p_y].piece = new Piece(q, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[4][p_y].piece = new Piece(k, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[5][p_y].piece = new Piece(b, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[6][p_y].piece = new Piece(n, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[7][p_y].piece = new Piece(r, i, init_piece_coord);
    }

    // creates pawns

    for (short int i = 0; i < 2; i++) {
        short int p_y{ i * 5 + 1 };

        for (short int x = 0; x < board_size; x++) {
            Coord init_piece_coord{ x, p_y };
            board[x][p_y].piece = new Piece(p, i, init_piece_coord);
        }
    }

    this->find_moves();
}

void Board::pop_move() {
    PastMove last_move = past_moves.back();
    
    BoardCell* board_cell = &board[last_move.to.x][last_move.to.y];

    // unpromotes the piece if it was promoted
    if (last_move.was_promoted) {
        board_cell->piece->piece_type = last_move.promoted_from;
    }

    board_cell->piece->piece_coord = last_move.from;
    board[last_move.from.x][last_move.from.y].piece = board_cell->piece;

    // untakes a piece if it was taken last move
    if (last_move.piece_taken != nullptr) {
        board_cell->piece = last_move.piece_taken;
    }
    else {
        board_cell->piece = nullptr;
    }

    --move_number;
    past_moves.erase(past_moves.end() - 1);

    this->find_moves();
}

bool Board::push_move(Move& player_move, bool check_for_pin /* = true */) {
    bool is_legal{ false };
    Move game_move;
    for (auto& legal_move : possible_moves) {
        if (legal_move == player_move) {
            is_legal = true;
            game_move = legal_move;
        }
    }
    if (is_legal == false) { return false; }

    PastMove past_move{ game_move.from, game_move.to };
    if (board[game_move.to.x][game_move.to.y].piece != nullptr) {
        past_move.piece_taken = board[game_move.to.x][game_move.to.y].piece;
    }

    past_moves.push_back(past_move);

    // removes the piece taken off the board (primary purpose for en passant as the position of the piece you take and position you move to are different)
    if (game_move.piece_taken != nullptr) {
        board[game_move.piece_taken->piece_coord.x][game_move.piece_taken->piece_coord.y].piece = nullptr;
    }

    board[game_move.to.x][game_move.to.y].piece = board[game_move.from.x][game_move.from.y].piece;
    board[game_move.from.x][game_move.from.y].piece = nullptr;

    board[game_move.to.x][game_move.to.y].piece->piece_coord = game_move.to;

    ++move_number;
    this->find_moves(check_for_pin);

    return true;
}

bool Board::is_pinned(Move& pin_move) {
    this->push_move(pin_move, false);
    bool king_is_pinned{ false };

    // finds the king on the board
    Coord king_coord{};
    for (short int y = 0; y < board_size; ++y) {
        for (short int x = 0; x < board_size; x++) {
            BoardCell board_cell{ board[x][y] };
            if (board_cell.piece != nullptr && board_cell.piece->piece_type == k) {
                king_coord = board_cell.coord;

                // checks if any of the enemy pieces can take the king on the following move
                for (auto& check_move_pin : pin_moves) {
                    if (check_move_pin.to == king_coord) {
                        king_is_pinned = true;
                    }
                }
            }
        }
    }

    

    return king_is_pinned;
}

void Board::add_move(Coord& move_to, Coord& piece_c, short int player_move_multiplier, bool check_for_pin, Piece* piece_taken /* = nullptr */) {

    Coord move_from{ piece_c.x, piece_c.y };

    Move possible_move{ move_from, move_to, piece_taken };


    if (check_for_pin) {
        if (!this->is_pinned(possible_move)) {
            possible_moves.push_back(possible_move);
        }
    }
    else {
        pin_moves.push_back(possible_move);
    }
}

void Board::find_moves(bool check_for_pin) {
    if (check_for_pin){
        possible_moves.clear();
    }
    else {
        pin_moves.clear();
    }
    short int player_move_multiplier { 1 };
    if (move_number % 2 == 1) {
        player_move_multiplier = -1;
    }

    short int knight_x[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    short int knight_y[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

    BoardCell* check_cell{ nullptr };

    short int check_x{};
    short int check_y{};

    for (short int y = 0; y < board_size; ++y) {
        for (short int x = 0; x < board_size; x++) {

            BoardCell board_cell = board[x][y];
            if (board_cell.piece != nullptr && board_cell.piece->owner == move_number % 2) {
                Coord& piece_c = board_cell.coord;

                switch (board_cell.piece->piece_type) {
                case p:
                    // move forward one place
                    check_x = piece_c.x;
                    check_y = piece_c.y + player_move_multiplier;
                    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                        check_cell = &board[check_x][check_y];
                        if (board[piece_c.x][piece_c.y + player_move_multiplier].piece == nullptr) {
                            Coord move_to{ piece_c.x, piece_c.y + player_move_multiplier };
                            add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                        }
                    }

                    // move forawrd two places
                    check_x = piece_c.x;
                    check_y = piece_c.y + (player_move_multiplier * 2);
                    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                        check_cell = &board[check_x][check_y];

                        if (board[piece_c.x][piece_c.y + (player_move_multiplier * 2)].piece == nullptr && piece_c.y == board_cell.piece->owner * 5 + 1) {
                            Coord move_to{ piece_c.x, piece_c.y + (player_move_multiplier * 2) };
                            add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                        }
                    }


                    // take piece to the right
                    check_x = piece_c.x + 1;
                    check_y = piece_c.y + player_move_multiplier;
                    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                        check_cell = &board[check_x][check_y];
                        if (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                            Coord move_to{ piece_c.x + 1, piece_c.y + player_move_multiplier };
                            add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                        }
                    }

                    // take piece to the left
                    check_x = piece_c.x - 1;
                    check_y = piece_c.y + player_move_multiplier;
                    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                        check_cell = &board[check_x][check_y];
                        if (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                            Coord move_to{ piece_c.x - 1, piece_c.y + player_move_multiplier };
                            add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                        }
                    }

                    if (move_number > 0) {
                        // en passant to the left
                        PastMove last_move = past_moves.back();

                        check_x = piece_c.x - 1;
                        check_y = piece_c.y;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner
                                && check_cell->piece->piece_type == p && last_move.to == check_cell->coord && last_move.from.y + (player_move_multiplier * 2) == check_x) {
                                Coord move_to{ piece_c.x - 1, piece_c.y + player_move_multiplier };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin, check_cell->piece);
                            }
                        }

                        // en passant to the right
                        check_x = piece_c.x + 1;
                        check_y = piece_c.y;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner
                                && check_cell->piece->piece_type == p && last_move.to == check_cell->coord && last_move.from.y - player_move_multiplier * 2 == check_y) {
                                Coord move_to{ piece_c.x + 1, piece_c.y + player_move_multiplier };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin, check_cell->piece);
                            }
                        }
                    }
                    break;



                case b:
                    // checks to the top right of the bishop for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x + i;
                        check_y = piece_c.y + i;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                            }
                        }
                        else { break; }
                    }

                    // checks to the top left of the bishop for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x - i;
                        check_y = piece_c.y + i;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                            }
                        }
                        else { break; }
                    }

                    // checks to the bottom right of the bishop for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x + i;
                        check_y = piece_c.y - i;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                            }
                        }
                        else { break; }
                    }

                    // checks to the bottom left of the bishop for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x - i;
                        check_y = piece_c.y - i;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                            }
                        }
                        else { break; }
                    }
                break;



                case n:

                    for (short int i = 0; i < board_size; i++) {
                        check_x = piece_c.x + knight_x[i];
                        check_y = piece_c.y + knight_y[i];
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                            }
                        }
                    }
                break;

                case r:
                    // checks to the top of the rook for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x;
                        check_y = piece_c.y + i;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                            }
                        }
                    }

                    // checks to the bottom of the rook for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x;
                        check_y = piece_c.y - i;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                            }
                        }
                    }

                    // checks to the left of the rook for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x - i;
                        check_y = piece_c.y;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                            }
                        }
                    }

                    // checks to the right of the rook for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x + i;
                        check_y = piece_c.y;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                            }
                        }
                    }
                }
            }
        }
    }
}

char whiteUpperCase(char input, short int owner) {
    //std::cout << owner << std::endl;
    if (owner == 1) {
        return input;
    }
    else {
        return std::toupper(input);
    }
}

std::ostream& operator<<(std::ostream& out, const Board& board_class) {
    std::cout << "========================" << std::endl << std::endl;

    for (short int y = 7; y > -1;  --y) {
        for (short int x = 0; x < board_size; x++) {
            const BoardCell& board_cell = board_class.board[x][y];
            const Piece* board_piece = board_cell.piece;
            if (board_piece == nullptr) {
                out << ' ' << '-' << ' ';
            }
            else {
                switch (board_piece->piece_type)
                {
                case p:
                    out << ' ' << whiteUpperCase('p', board_piece->owner) << ' ';
                    break;
                case r:
                    out << ' ' << whiteUpperCase('r', board_piece->owner) << ' ';
                    break;
                case n:
                    out << ' ' << whiteUpperCase('n', board_piece->owner) << ' ';
                    break;
                case b:
                    out << ' ' << whiteUpperCase('b', board_piece->owner) << ' ';
                    break;
                case q:
                    out << ' ' << whiteUpperCase('q', board_piece->owner) << ' ';
                    break;
                case k:
                    out << ' ' << whiteUpperCase('k', board_piece->owner) << ' ';
                    break;
                default:
                    out << ' ' << ' ' << ' ';
                    break;
                }
            }
        }
        out << std::endl;
    }
    return out;
}

  
Board::~Board()
{
    for (short int y = 7; y > -1; --y) {
        for (short int x = 0; x < board_size; x++) {
            const BoardCell& board_cell = this->board[x][y];
            const Piece* board_piece = board_cell.piece;
            delete board_piece;
            board_piece = nullptr;
        }
    }
}