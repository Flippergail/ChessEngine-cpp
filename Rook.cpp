#include "Rook.h"



void Rook::add_moves(Board& board, short int player_move_multiplier, bool check_for_pin, BoardCell& board_cell) {
    Coord piece_c = this->piece_coord;
    short int check_x = piece_c.x;
    short int check_y = piece_c.y + player_move_multiplier;
    BoardCell* check_cell{ nullptr };

    // checks to the top of the rook for possible moves
    for (short int i = 0; i < board_size; i++)
    {
        check_x = piece_c.x;
        check_y = piece_c.y + i;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                Coord move_to{ check_x, check_y };
                board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                if (check_cell->piece != nullptr) { break; }
            }
            else { break; }
        }
        else { break; }
    }

    // checks to the bottom of the rook for possible moves
    for (short int i = 0; i < board_size; i++)
    {
        check_x = piece_c.x;
        check_y = piece_c.y - i;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                Coord move_to{ check_x, check_y };
                board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                if (check_cell->piece != nullptr) { break; }
            }
            else { break; }
        }
        else { break; }
    }

    // checks to the left of the rook for possible moves
    for (short int i = 0; i < board_size; i++)
    {
        check_x = piece_c.x - i;
        check_y = piece_c.y;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                Coord move_to{ check_x, check_y };
                board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                if (check_cell->piece != nullptr) { break; }
            }
            else { break; }
        }
        else { break; }
    }

    // checks to the right of the rook for possible moves
    for (short int i = 0; i < board_size; i++)
    {
        check_x = piece_c.x + i;
        check_y = piece_c.y;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                Coord move_to{ check_x, check_y };
                board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                if (check_cell->piece != nullptr) { break; }
            }
            else { break; }
        }
        else { break; }
    }
}