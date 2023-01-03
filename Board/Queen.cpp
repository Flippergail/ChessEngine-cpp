#include "Queen.h"


void Queen::add_moves(Board& board, int player_move_multiplier, bool check_for_pin, BoardCell& board_cell) {
    Coord piece_c = this->piece_coord;
    int check_x = piece_c.x;
    int check_y = piece_c.y + player_move_multiplier;
    Coord move_to{};
    BoardCell* check_cell{ nullptr };

    // checks to the top of the queen for possible moves
    for (int i = 1; i < board_size + 1; i++)
    {
        check_x = piece_c.x;
        check_y = piece_c.y + i;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || ( check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner ) ) {
                move_to.x = check_x;
                move_to.y = check_y;
                if (check_cell->piece != nullptr) {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
                    break;
                }
                else {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
                }
            }
            else { break; }
        }
        else { break; }
    }

    // checks to the bottom of the queen for possible moves
    for (int i = 1; i < board_size + 1; i++)
    {
        check_x = piece_c.x;
        check_y = piece_c.y - i;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                move_to.x = check_x;
                move_to.y = check_y;
                if (check_cell->piece != nullptr) {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
                    break;
                }
                else {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
                }
            }
            else { break; }
        }
        else { break; }
    }

    // checks to the left of the queen for possible moves
    for (int i = 1; i < board_size + 1; i++)
    {
        check_x = piece_c.x - i;
        check_y = piece_c.y;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                move_to.x = check_x;
                move_to.y = check_y;
                if (check_cell->piece != nullptr) {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
                    break;
                }
                else {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
                }
            }
            else { break; }
        }
        else { break; }
    }

    // checks to the right of the queen for possible moves
    for (int i = 1; i < board_size + 1; i++)
    {
        check_x = piece_c.x + i;
        check_y = piece_c.y;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                move_to.x = check_x;
                move_to.y = check_y;;
                if (check_cell->piece != nullptr) {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
                    break;
                }
                else {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
                }
            }
            else { break; }
        }
        else { break; }
    }

    // checks to the top right of the queen for possible moves
    for (int i = 1; i < board_size + 1; i++)
    {
        check_x = piece_c.x + i;
        check_y = piece_c.y + i;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                move_to.x = check_x;
                move_to.y = check_y;
                if (check_cell->piece != nullptr) {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
                    break;
                }
                else {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
                }
            }
            else { break; }
        }
        else { break; }
    }

    // checks to the top left of the queen for possible moves
    for (int i = 1; i < board_size + 1; i++)
    {
        check_x = piece_c.x - i;
        check_y = piece_c.y + i;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                move_to.x = check_x;
                move_to.y = check_y;
                if (check_cell->piece != nullptr) {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
                    break;
                }
                else {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
                }
            }
            else { break; }
        }
        else { break; }
    }

    // checks to the bottom right of the queen for possible moves
    for (int i = 1; i < board_size + 1; i++)
    {
        check_x = piece_c.x + i;
        check_y = piece_c.y - i;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                move_to.x = check_x;
                move_to.y = check_y;
                if (check_cell->piece != nullptr) {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
                    break;
                }
                else {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
                }
            }
            else { break; }
        }
        else { break; }
    }

    // checks to the bottom left of the queen for possible moves
    for (int i = 1; i < board_size + 1; i++)
    {
        check_x = piece_c.x - i;
        check_y = piece_c.y - i;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
                move_to.x = check_x;
                move_to.y = check_y;
                if (check_cell->piece != nullptr) {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
                    break;
                }
                else {
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
                }
            }
            else { break; }
        }
        else { break; }
    }
}