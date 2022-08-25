#include "Knight.h"

short int knight_x[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
short int knight_y[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

void Knight::add_moves(Board& board, short int player_move_multiplier, bool check_for_pin, BoardCell& board_cell) {
    Coord piece_c = this->piece_coord;
    short int check_x{};
    short int check_y{};

    for (short int i = 0; i < board_size; i++) {
        check_x = piece_c.x + knight_x[i];
        check_y = piece_c.y + knight_y[i];
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            if (board.board[check_x][check_y].piece == nullptr || board.board[check_x][check_y].piece != nullptr && board.board[check_x][check_y].piece->owner != board_cell.piece->owner) {
                Coord move_to{ check_x, check_y };
                board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
            }
        }
    }
}