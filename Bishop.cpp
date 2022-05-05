#include "Bishop.h"
#include <iostream>

void Bishop::add_moves(Board& board, short int player_move_multiplier, bool check_for_pin, BoardCell& board_cell)
{
    std::cout << "hit" << std::endl;
    Coord piece_c = this->piece_coord;
    short int check_x{};
    short int check_y{};
    BoardCell* check_cell{ nullptr };

    // checks to the top right of the bishop for possible moves
    for (short int i = 0; i < board_size; i++)
    {
        check_x = piece_c.x + i;
        check_y = piece_c.y + i;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                Coord move_to{ check_x, check_y };
                board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                if (check_cell->piece != nullptr) { break; }
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
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                Coord move_to{ check_x, check_y };
                board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                if (check_cell->piece != nullptr) { break; }
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
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                Coord move_to{ check_x, check_y };
                board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                if (check_cell->piece != nullptr) { break; }
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
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                Coord move_to{ check_x, check_y };
                board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
                if (check_cell->piece != nullptr) { break; }
            }
            else { break; }
        }
    }
}