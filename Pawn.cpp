#include "Pawn.h"



void Pawn::add_moves(Board& board, short int player_move_multiplier, bool check_for_pin, BoardCell& board_cell)
{
    // move forward one place
    Coord piece_c = this->piece_coord;
    short int check_x = piece_c.x;
    short int check_y = piece_c.y + player_move_multiplier;
    BoardCell* check_cell{ nullptr };
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (board.board[piece_c.x][piece_c.y + player_move_multiplier].piece == nullptr) {
            Coord move_to{ piece_c.x, piece_c.y + player_move_multiplier };
            board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
        }
    }

    // move forawrd two places
    check_x = piece_c.x;
    check_y = piece_c.y + (player_move_multiplier * 2);
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];

        if (board.board[piece_c.x][piece_c.y + (player_move_multiplier * 2)].piece == nullptr && piece_c.y == board_cell.piece->owner * 5 + 1
            && board.board[piece_c.x][piece_c.y + (player_move_multiplier)].piece == nullptr) {
            Coord move_to{ piece_c.x, piece_c.y + (player_move_multiplier * 2) };
            board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
        }
    }


    // take piece to the right
    check_x = piece_c.x + 1;
    check_y = piece_c.y + player_move_multiplier;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
            Coord move_to{ piece_c.x + 1, piece_c.y + player_move_multiplier };
            board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
        }
    }

    // take piece to the left
    check_x = piece_c.x - 1;
    check_y = piece_c.y + player_move_multiplier;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
            Coord move_to{ piece_c.x - 1, piece_c.y + player_move_multiplier };
            board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
        }
    }

    if (board.move_number > 0) {
        // en passant to the left
        PastMove last_move = board.past_moves.back();

        check_x = piece_c.x - 1;
        check_y = piece_c.y;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner
                && check_cell->piece->piece_type == p && last_move.to == check_cell->coord && last_move.from.y + (player_move_multiplier * 2) == check_x) {
                Coord move_to{ piece_c.x - 1, piece_c.y + player_move_multiplier };
                board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin, check_cell->piece);
            }
        }

        // en passant to the right
        check_x = piece_c.x + 1;
        check_y = piece_c.y;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner
                && check_cell->piece->piece_type == p && last_move.to == check_cell->coord && last_move.from.y - player_move_multiplier * 2 == check_y) {
                Coord move_to{ piece_c.x + 1, piece_c.y + player_move_multiplier };
                board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin, check_cell->piece);
            }
        }
    }
}
