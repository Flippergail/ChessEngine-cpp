#include "Board.h"
#include "Pawn.h"
#include "Bishop.h"

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
        short int p_y{ i * 7 };
        Coord init_piece_coord{ 0, p_y };
        board[0][p_y].piece = new Piece(r, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[1][p_y].piece = new Piece(n, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[2][p_y].piece = new Bishop(b, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[3][p_y].piece = new Piece(q, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[4][p_y].piece = new Piece(k, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[5][p_y].piece = new Bishop(b, i, init_piece_coord);
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
            board[x][p_y].piece = new Pawn(p, i, init_piece_coord);
        }
    }

    this->find_moves();
}

void Board::pop_move(bool check_moves /* = true*/) {
    game_state = playing_state;

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

    // moves rook back if it was a castle
    if (last_move.was_castle) {
        if (last_move.to.x == 6) {
            // undo short castle
            Coord castle_r_coord{ 7, last_move.to.y };
            board[7][last_move.from.y].piece = board[5][last_move.from.y].piece;
            board[5][last_move.from.y].piece = nullptr;
            board[7][last_move.from.y].piece->piece_coord = castle_r_coord;
        }
        else if (last_move.to.x == 2) {
            // undo long castle
            Coord castle_r_coord{ 0, last_move.to.y };
            board[0][last_move.from.y].piece = board[3][last_move.from.y].piece;
            board[3][last_move.from.y].piece = nullptr;
            board[0][last_move.from.y].piece->piece_coord = castle_r_coord;
        }
    }

    --move_number;
    past_moves.erase(past_moves.end() - 1);

    if (check_moves) {
        this->find_moves();
    }
}

bool Board::push_move(Move& player_move, bool check_for_pin /* = true */) {
    if (game_state != playing_state) { return false; }

    bool is_legal{ false };
    Move game_move{player_move};
    for (auto& legal_move : possible_moves) {
        if (legal_move == player_move) {
            is_legal = true;
            game_move = legal_move;
        }
    }
    if (is_legal == false && check_for_pin) { return false; }

    PastMove past_move{ game_move.from, game_move.to, nullptr, game_move.is_promotion, game_move.is_castle };
    if (board[game_move.to.x][game_move.to.y].piece != nullptr) {
        past_move.piece_taken = board[game_move.to.x][game_move.to.y].piece;
    }

    past_moves.push_back(past_move);
    //std::cout << "push_back : " << past_move.from.x << ", " << past_move.from.y << std::endl;
    // removes the piece taken off the board (primary purpose for en passant as the position of the piece you take and position you move to are different)
    if (game_move.piece_taken != nullptr) {
        board[game_move.piece_taken->piece_coord.x][game_move.piece_taken->piece_coord.y].piece = nullptr;
    }

    board[game_move.to.x][game_move.to.y].piece = board[game_move.from.x][game_move.from.y].piece;
    board[game_move.from.x][game_move.from.y].piece = nullptr;
    board[game_move.to.x][game_move.to.y].piece->piece_coord = game_move.to;

    if (game_move.is_castle) {
        if (game_move.to.x == 6) {
            // short castle
            board[5][game_move.to.y].piece = board[7][game_move.to.y].piece;
            board[7][game_move.to.y].piece = nullptr;
            Coord castle_r_coord{ 5, game_move.to.y };
            board[5][game_move.to.y].piece->piece_coord = castle_r_coord;
        }
        else if(game_move.to.x == 2) {
            // long castle
            board[3][game_move.to.y].piece = board[0][game_move.to.y].piece;
            board[0][game_move.to.y].piece = nullptr;
            Coord castle_r_coord{ 3, game_move.to.y };
            board[3][game_move.to.y].piece->piece_coord = castle_r_coord;
        }
    }


    ++move_number;
    this->find_moves(check_for_pin);

    return true;
}

bool Board::is_move_legal(Move& check_move) {

    // checks if there is a check after the piece has moved
    this->push_move(check_move, false);
    bool move_is_legal{ !this->in_check()};
    this->pop_move(false);

    return move_is_legal;
}


bool Board::in_check(){
    bool is_check{ false };

    // finds the king on the board
    Coord king_coord{};
    for (short int y = 0; y < board_size; ++y) {
        for (short int x = 0; x < board_size; x++) {
            BoardCell board_cell{ board[x][y] };
            if (board_cell.piece != nullptr && board_cell.piece->piece_type == k && board_cell.piece->owner == (move_number - 1) % 2) {
                king_coord = board_cell.coord;

                // checks if any of the enemy pieces can take the king on the following move
                for (auto& check_move_pin : pin_moves) {
                    if (check_move_pin.to == king_coord) {
                        is_check = true;
                    }
                }
            }
        }
    }

    return is_check;
}

void Board::add_move(Coord& move_to, Coord& piece_c, short int player_move_multiplier, bool check_for_pin, Piece* piece_taken /* = nullptr */, bool is_castle /* = false*/) {

    Move possible_move{ piece_c, move_to, piece_taken, false,  is_castle };
    
    if (check_for_pin) {
        if (this->is_move_legal(possible_move)) {
            possible_moves.push_back(possible_move);
        }
    }
    else {
        pin_moves.push_back(possible_move);
    }
}

void Board::find_moves(bool check_for_pin /* = true*/) {
    if (check_for_pin) {
        possible_moves.clear();
    }
    else {
        pin_moves.clear();
    }
    short int player_move_multiplier{ 1 };
    if (move_number % 2 == 1) {
        player_move_multiplier = -1;
    }

    short int knight_x[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    short int knight_y[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

    BoardCell* check_cell{ nullptr };

    short int check_x{};
    short int check_y{};

    // TODO: remove when all refactoring done with Piece subclass
    bool use_new_piece_class = true;

    for (short int y = 0; y < board_size; ++y) {
        for (short int x = 0; x < board_size; x++) {

            BoardCell board_cell = board[x][y];
            if (board_cell.piece != nullptr && board_cell.piece->owner == move_number % 2) {
                Coord& piece_c = board_cell.coord;

                switch (board_cell.piece->piece_type) {
                case p:
                    board_cell.piece->add_moves(*this, player_move_multiplier, check_for_pin, board_cell);
                    break;



                case b:
                    board_cell.piece->add_moves(*this, player_move_multiplier, check_for_pin, board_cell);
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
                                if (check_cell->piece != nullptr) { break; }
                            }
                        }
                        else { break; }
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
                                if (check_cell->piece != nullptr) { break; }
                            }
                        }
                        else { break; }
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
                                if (check_cell->piece != nullptr) { break; }
                            }
                        }
                        else { break; }
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
                                if (check_cell->piece != nullptr) { break; }
                            }
                        }
                        else { break; }
                    }
                    break;
                case q:
                    // checks to the top of the queen for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x;
                        check_y = piece_c.y + i;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                                if (check_cell->piece != nullptr) { break; }
                            }
                        }
                        else { break; }
                    }

                    // checks to the bottom of the queen for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x;
                        check_y = piece_c.y - i;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                                if (check_cell->piece != nullptr) { break; }
                            }
                        }
                        else { break; }
                    }

                    // checks to the left of the queen for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x - i;
                        check_y = piece_c.y;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                                if (check_cell->piece != nullptr) { break; }
                            }
                        }
                        else { break; }
                    }

                    // checks to the right of the queen for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x + i;
                        check_y = piece_c.y;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                                if (check_cell->piece != nullptr) { break; }
                            }
                        }
                        else { break; }
                    }

                    // checks to the top right of the queen for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x + i;
                        check_y = piece_c.y + i;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                                if (check_cell->piece != nullptr) { break; }
                            }
                        }
                        else { break; }
                    }

                    // checks to the top left of the queen for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x - i;
                        check_y = piece_c.y + i;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                                if (check_cell->piece != nullptr) { break; }
                            }
                        }
                        else { break; }
                    }

                    // checks to the bottom right of the queen for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x + i;
                        check_y = piece_c.y - i;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                                if (check_cell->piece != nullptr) { break; }
                            }
                        }
                        else { break; }
                    }

                    // checks to the bottom left of the queen for possible moves
                    for (short int i = 0; i < board_size; i++)
                    {
                        check_x = piece_c.x - i;
                        check_y = piece_c.y - i;
                        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                            check_cell = &board[check_x][check_y];
                            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                                Coord move_to{ check_x, check_y };
                                add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                                if (check_cell->piece != nullptr) { break; }
                            }
                        }
                        else { break; }
                    }
                    break;
                case k:
                    // checks to the top of the king for possible moves
                    check_x = piece_c.x;
                    check_y = piece_c.y + 1;
                    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                        check_cell = &board[check_x][check_y];
                        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                            Coord move_to{ check_x, check_y };
                            add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                        }
                    }

                    // checks to the bottom of the king for possible moves
                    check_x = piece_c.x;
                    check_y = piece_c.y - 1;
                    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                        check_cell = &board[check_x][check_y];
                        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                            Coord move_to{ check_x, check_y };
                            add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                        }
                    }

                    // checks to the left of the king for possible moves
                    check_x = piece_c.x - 1;
                    check_y = piece_c.y;
                    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                        check_cell = &board[check_x][check_y];
                        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                            Coord move_to{ check_x, check_y };
                            add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                        }
                    }

                    // checks to the right of the king for possible moves
                    check_x = piece_c.x + 1;
                    check_y = piece_c.y;
                    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                        check_cell = &board[check_x][check_y];
                        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                            Coord move_to{ check_x, check_y };
                            add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                        }
                    }

                    // checks to the top right of the king for possible moves
                    check_x = piece_c.x + 1;
                    check_y = piece_c.y + 1;
                    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                        check_cell = &board[check_x][check_y];
                        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                            Coord move_to{ check_x, check_y };
                            add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                        }
                    }

                    // checks to the top left of the king for possible moves
                    check_x = piece_c.x - 1;
                    check_y = piece_c.y + 1;
                    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                        check_cell = &board[check_x][check_y];
                        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                            Coord move_to{ check_x, check_y };
                            add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                        }
                    }

                    // checks to the bottom right of the king for possible moves
                    check_x = piece_c.x + 1;
                    check_y = piece_c.y - 1;
                    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                        check_cell = &board[check_x][check_y];
                        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                            Coord move_to{ check_x, check_y };
                            add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                        }
                    }

                    // checks to the bottom left of the king for possible moves
                    check_x = piece_c.x - 1;
                    check_y = piece_c.y - 1;
                    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
                        check_cell = &board[check_x][check_y];
                        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                            Coord move_to{ check_x, check_y };
                            add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                        }
                    
                    }

                    if (check_for_pin) {

                        // for castling
                        bool king_moved{ false };
                        for (auto& past_move : past_moves) {
                            if (past_move.from == board[4][(move_number % 2) * 7].coord) {
                                std::cout << "king has moved : " << past_move.to.x << ", " << past_move.to.y << std::endl;
                                king_moved = true;
                            }
                        }

                        if (!king_moved) {

                            // checks for short castle
                            bool short_r_moved{ false };
                            for (auto& past_move : past_moves) {
                                if (past_move.from == board[7][(move_number % 2) * 7].coord) {
                                    short_r_moved = true;
                                }
                            }

                            // checks for long castle
                            bool long_r_moved{ false };
                            for (auto& past_move : past_moves) {
                                if (past_move.from == board[0][(move_number % 2) * 7].coord) {
                                    long_r_moved = true;
                                }
                            }

                            // short castle
                            if (!short_r_moved) {
                                check_cell = &board[piece_c.x + 1][piece_c.y];
                                if (check_cell->piece == nullptr) {
                                    std::cout << "passed" << std::endl;
                                    Coord move_to{ piece_c.x + 1, piece_c.y };
                                    Move possible_move{ piece_c, move_to };
                                    if (is_move_legal(possible_move)) {
                                        std::cout << "move legal" << std::endl;
                                        ++move_to.x;
                                        possible_move.to = move_to;
                                        add_move(move_to, piece_c, player_move_multiplier, true, nullptr, true);
                                    }
                                }
                            }

                            // long castle
                            if (!long_r_moved) {
                                check_cell = &board[piece_c.x - 1][piece_c.y];
                                if (check_cell->piece == nullptr && board[1][piece_c.y].piece == nullptr) {
                                    Coord move_to{ piece_c.x - 1, piece_c.y };
                                    Move possible_move{ piece_c, move_to };
                                    if (is_move_legal(possible_move)) {
                                        --move_to.x;
                                        possible_move.to = move_to;
                                        add_move(move_to, piece_c, player_move_multiplier, true, nullptr, true);
                                    }
                                }
                            }

                        }
                    }
                }
            }
        }
    }

    // checks if the game is over
    if (size(possible_moves) == 0 && move_number > 1 && check_for_pin == true) {
        ++move_number;
        if (in_check()) {
            // checkmate
            game_state = move_number % 2;
            std::cout << game_state << " has won by checkmate" << std::endl;
        }
        else {
            // draw
            game_state = draw;
            std::cout << "draw" << std::endl;
        }
        --move_number;
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