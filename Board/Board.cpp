#include <algorithm>
#include <iterator>
#include <array>

#include "Board.h"

// include piece sub classes
#include "Pawn.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

Board::Board()
{

    // creates board cells
    for (int y = 0; y < board_size; ++y)
    {
        for (int x = 0; x < board_size; x++)
        {
            board[x][y].coord = Coord{x, y};
        }
    }

    // creates main pieces

    for (int i = 0; i < 2; i++)
    {   
        int p_y{i * (short)7};
        Coord init_piece_coord{0, p_y};
        board[0][p_y].piece = new Rook(r, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[1][p_y].piece = new Knight(n, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[2][p_y].piece = new Bishop(b, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[3][p_y].piece = new Queen(q, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[4][p_y].piece = new King(k, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[5][p_y].piece = new Bishop(b, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[6][p_y].piece = new Knight(n, i, init_piece_coord);
        init_piece_coord.x += 1;
        board[7][p_y].piece = new Rook(r, i, init_piece_coord);
    }

    // creates pawns

    for (int i = 0; i < 2; i++)
    {
        int p_y{i * 5 + 1};

        for (int x = 0; x < board_size; x++)
        {
            Coord init_piece_coord{x, p_y};
            board[x][p_y].piece = new Pawn(p, i, init_piece_coord);
        }
    }

    this->find_moves();
}

void Board::new_piece(BoardCell &board_cell, int piece_type, int piece_owner)
{
    switch (piece_type)
    {
    case p:
        board_cell.piece = new Pawn(p, piece_owner, board_cell.coord);
        break;
    case b:
        board_cell.piece = new Bishop(b, piece_owner, board_cell.coord);
        break;
    case n:
        board_cell.piece = new Knight(n, piece_owner, board_cell.coord);
        break;
    case r:
        board_cell.piece = new Rook(r, piece_owner, board_cell.coord);
        break;
    case q:
        board_cell.piece = new Queen(q, piece_owner, board_cell.coord);
    }
}

void Board::pop_move(bool check_moves /* = true*/)
{
    game_state = playing_state;
    PastMove last_move = past_moves.back();
    BoardCell *board_cell = &board[last_move.to.x][last_move.to.y];

    // unpromotes the piece if it was promoted
    if (last_move.promoted_to != 0)
    {
        board_cell->piece->~Piece();
        this->new_piece(*board_cell, last_move.promoted_from, (move_number - 1) % 2);
    }

    board_cell->piece->piece_coord = last_move.from;
    board[last_move.from.x][last_move.from.y].piece = board_cell->piece;
    board[last_move.to.x][last_move.to.y].piece = nullptr;

    // untakes a piece if it was taken last move
    if (last_move.piece_taken != 0)
    {
        this->new_piece(board[last_move.piece_taken_coord.x][last_move.piece_taken_coord.y], last_move.piece_taken, move_number % 2);
    }
    else
    {
        board_cell->piece = nullptr;
    }

    // moves rook back if it was a castle
    if (last_move.was_castle)
    {
        if (last_move.to.x == 6)
        {
            // undo short castle
            Coord castle_r_coord{7, last_move.to.y};
            board[7][last_move.from.y].piece = board[5][last_move.from.y].piece;
            board[7][last_move.from.y].piece->piece_coord = castle_r_coord;
            board[5][last_move.from.y].piece = nullptr;
        }
        else if (last_move.to.x == 2)
        {
            // undo long castle
            Coord castle_r_coord{0, last_move.to.y};
            board[0][last_move.from.y].piece = board[3][last_move.from.y].piece;
            board[0][last_move.from.y].piece->piece_coord = castle_r_coord; 
            board[3][last_move.from.y].piece = nullptr;
        }
    }

    --move_number;
    past_moves.erase(past_moves.end() - 1);

    if (check_moves)
    {
        this->find_moves();
    }
}

bool Board::push_move(Move &player_move, bool check_for_pin /* = true */)
{

    if (game_state != playing_state)
    {
        return false;
    }

    bool is_legal{false};
    Move game_move{player_move};
    for (auto &legal_move : possible_moves)
    {
        if (legal_move == player_move)
        {
            is_legal = true;
            game_move = legal_move;
        }
    }
    if (is_legal == false && check_for_pin)
    {
        return false;
    }

    PastMove past_move{game_move.from, game_move.to, {}, {}, game_move.promotion_to, game_move.is_castle, board[game_move.from.x][game_move.from.y].piece->piece_type};
    if (board[game_move.to.x][game_move.to.y].piece != nullptr)
    {
        past_move.piece_taken = board[game_move.to.x][game_move.to.y].piece->piece_type;
        past_move.piece_taken_coord = game_move.to;

        board[game_move.to.x][game_move.to.y].piece->~Piece();
    }

    // removes the piece taken off the board (primary purpose for en passant as the position of the piece you take and position you move to are different)
    if (game_move.piece_taken != nullptr)
    {
        past_move.piece_taken = game_move.piece_taken->piece_type;
        past_move.piece_taken_coord = game_move.piece_taken->piece_coord;

        board[game_move.piece_taken->piece_coord.x][game_move.piece_taken->piece_coord.y].piece->~Piece();
        board[game_move.piece_taken->piece_coord.x][game_move.piece_taken->piece_coord.y].piece = nullptr;
    }

    board[game_move.to.x][game_move.to.y].piece = board[game_move.from.x][game_move.from.y].piece;
    board[game_move.from.x][game_move.from.y].piece = nullptr;
    board[game_move.to.x][game_move.to.y].piece->piece_coord = game_move.to;

    past_moves.push_back(past_move);

    if (player_move.promotion_to != 0)
    {
        board[game_move.to.x][game_move.to.y].piece->~Piece();
        this->new_piece(board[game_move.to.x][game_move.to.y], player_move.promotion_to, move_number % 2);
    }

    if (game_move.is_castle)
    {
        if (game_move.to.x == 6)
        {
            // short castle
            board[5][game_move.to.y].piece = board[7][game_move.to.y].piece;
            board[7][game_move.to.y].piece = nullptr;
            Coord castle_r_coord{5, game_move.to.y};
            board[5][game_move.to.y].piece->piece_coord = castle_r_coord;
        }
        else if (game_move.to.x == 2)
        {
            // long castle
            board[3][game_move.to.y].piece = board[0][game_move.to.y].piece;
            board[0][game_move.to.y].piece = nullptr;
            Coord castle_r_coord{3, game_move.to.y};
            board[3][game_move.to.y].piece->piece_coord = castle_r_coord;
        }
    }

    ++move_number;
    this->find_moves(check_for_pin);

    return true;
}

bool Board::is_move_legal(Move &check_move)
{

    // checks if there is a check after the piece has moved
    this->push_move(check_move, false);
    bool move_is_legal{!this->in_check()};
    this->pop_move(false);

    return move_is_legal;
}

bool Board::in_check()
{
    bool is_check{false};

    // finds the king on the board
    Coord king_coord{};
    for (int y = 0; y < board_size; ++y)
    {
        for (int x = 0; x < board_size; x++)
        {
            BoardCell &board_cell{board[x][y]};
            if (board_cell.piece != nullptr && board_cell.piece->piece_type == k && board_cell.piece->owner == (move_number - 1) % 2)
            {
                king_coord = board_cell.coord;

                // checks if any of the enemy pieces can take the king on the following move
                for (auto &check_move_pin : pin_moves)
                {
                    if (check_move_pin.to == king_coord)
                    {
                        is_check = true;
                    }
                }
            }
        }
    }

    return is_check;
}

void Board::add_move(Coord &move_to, Coord &piece_c, int player_move_multiplier, bool check_for_pin, Piece *piece_taken /* = nullptr*/, int promotion_to /* = 0*/, bool is_castle /* = false*/)
{

    Move possible_move{piece_c, move_to, piece_taken, promotion_to, is_castle};

    if (check_for_pin)
    {
        if (this->is_move_legal(possible_move))
        {
            possible_moves.push_back(possible_move);
        }
    }
    else
    {
        pin_moves.push_back(possible_move);
    }
}

bool Board::are_only_pieces(int white_piece, int black_piece)
{
    if (white_piece == 0)
    {
        if (this->white_pieces.size() != 1)
        {
            return false;
        }
    }

    if (black_piece == 0)
    {
        if (this->black_pieces.size() != 1)
        {
            return false;
        }
    }

    if (std::count(this->white_pieces.begin(), this->white_pieces.end(), white_piece) == 0)
    {
        return false;
    }

    if (std::count(this->black_pieces.begin(), this->black_pieces.end(), black_piece) == 0)
    {
        return false;
    }

    return true;
}

void Board::find_moves(bool check_for_pin /* = true*/)
{
    if (check_for_pin)
    {
        possible_moves.clear();
    }
    else
    {
        pin_moves.clear();
    }
    int player_move_multiplier{1};
    if (move_number % 2 == 1)
    {
        player_move_multiplier = -1;
    }

    BoardCell *check_cell{nullptr};

    int check_x{};
    int check_y{};

    for (int y = 0; y < board_size; ++y)
    {
        for (int x = 0; x < board_size; x++)
        {

            BoardCell board_cell = board[x][y];
            if (board_cell.piece != nullptr && board_cell.piece->owner == move_number % 2)
            {
                Coord &piece_c = board_cell.coord;
                board_cell.piece->add_moves(*this, player_move_multiplier, check_for_pin, board_cell);
            }
        }
    }

    // checks if the game is over
    if (size(possible_moves) == 0 && move_number > 1 && check_for_pin == true)
    {
        ++move_number;
        if (in_check())
        {
            // checkmate
            game_state = move_number % 2;
            //std::cout << game_state << " has won by checkmate" << std::endl;
        }
        else
        {
            // draw
            game_state = draw;
        }
        --move_number;
    }

    // insufficient material draw

    // gets all the pieces on the board
    this->white_pieces.clear();
    this->black_pieces.clear();

    for (int y = 0; y < board_size; ++y)
    {
        for (int x = 0; x < board_size; x++)
        {
            BoardCell &board_cell{board[x][y]};
            if (board_cell.piece != nullptr)
            {
                if (board_cell.piece->owner == 0)
                {
                    this->white_pieces.push_back(board_cell.piece->piece_type);
                }
                else if (board_cell.piece->owner == 1)
                {
                    this->black_pieces.push_back(board_cell.piece->piece_type);
                }
            }
        }
    }

    // checks for insufficient material scenarios
    if (this->white_pieces.size() <= 3 || this->black_pieces.size() <= 3)
    {
        if (this->are_only_pieces(0, 0) /* (k, k) */)
        {
            game_state = draw;
        }
        else if (this->are_only_pieces(b, b))
        {
            game_state = draw;
        }
        else if (this->are_only_pieces(q, q))
        {
            game_state = draw;
        }
        else if (this->are_only_pieces(r, r))
        {
            game_state = draw;
        }
        else if (this->are_only_pieces(b, 0))
        {
            game_state = draw;
        }
        else if (this->are_only_pieces(0, b))
        {
            game_state = draw;
        }

        // checks for two knights draw
        bool only_knights = true;
        for (int white_piece : this->white_pieces)
        {
            if (white_piece != n and white_piece != k)
            {
                only_knights = false;
            }
        }
        for (int black_piece : this->black_pieces)
        {
            if (black_piece != n and black_piece != k)
            {
                only_knights = false;
            }
        }
        if (only_knights)
        {
            game_state = draw;
        }
    }
}

char whiteUpperCase(char input, int owner)
{
    if (owner == 1)
    {
        return input;
    }
    else
    {
        return std::toupper(input);
    }
}

std::ostream &operator<<(std::ostream &out, const Board &board_class)
{
    std::cout << "========================" << std::endl
              << std::endl;

    for (int y = 7; y > -1; --y)
    {
        for (int x = 0; x < board_size; x++)
        {
            const BoardCell &board_cell = board_class.board[x][y];
            const Piece *board_piece = board_cell.piece;
            if (board_piece == nullptr)
            {
                out << ' ' << '-' << ' ';
            }
            else
            {
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
    for (int y = 7; y > -1; --y)
    {
        for (int x = 0; x < board_size; x++)
        {
            const BoardCell &board_cell = this->board[x][y];
            const Piece *board_piece = board_cell.piece;
            delete board_piece;
            board_piece = nullptr;
        }
    }
}