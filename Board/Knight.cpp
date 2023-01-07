#include "Knight.h"

int knight_x[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
int knight_y[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

void Knight::add_moves(Board& board, int player_move_multiplier, bool check_for_pin, BoardCell& board_cell) {
    Coord piece_c = this->piece_coord;
    int check_x{};
    int check_y{};
    Coord move_to{};

    for (int i = 0; i < board_size; i++) {
        check_x = piece_c.x + knight_x[i];
        check_y = piece_c.y + knight_y[i];
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            if (board.board[check_x][check_y].piece == nullptr || board.board[check_x][check_y].piece != nullptr && board.board[check_x][check_y].piece->owner != board_cell.piece->owner) {
                move_to.x = check_x;
                move_to.y = check_y;
                if (board.board[check_x][check_y].piece != nullptr) {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
                }
                else {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
                }
            }
        }
    }
}