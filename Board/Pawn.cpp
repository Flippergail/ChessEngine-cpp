#include "Pawn.h"


void Pawn::add_moves(Board& board, int player_move_multiplier, bool check_for_pin, BoardCell& board_cell)
{
    BoardCell* check_cell{ nullptr };
    Coord piece_c = this->piece_coord;
    Coord move_to{};

    // move forward one place
    int check_x = piece_c.x;
    int check_y = piece_c.y + player_move_multiplier;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (board.board[piece_c.x][piece_c.y + player_move_multiplier].piece == nullptr) {
            move_to.x = check_x;
            move_to.y = check_y;
            
            // checks if the pawn can promote
            if (move_to.y == (board.move_number + 1) % 2 * 7) {
                // adds possible promotions of pawn
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, false, check_for_pin, b);
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, false, check_for_pin, n);
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, false, check_for_pin, r);
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, false, check_for_pin, q);
            }
            else {
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, false, check_for_pin);
            }
        }
    }

    // move forawrd two places
    check_x = piece_c.x;
    check_y = piece_c.y + (player_move_multiplier * 2);
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];

        if (board.board[piece_c.x][piece_c.y + (player_move_multiplier * 2)].piece == nullptr && piece_c.y == board_cell.piece->owner * 5 + 1
            && board.board[piece_c.x][piece_c.y + (player_move_multiplier)].piece == nullptr) {
            move_to.x = check_x;
            move_to.y = check_y;
            board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, false, check_for_pin);
        }
    }


    // take piece to the right
    check_x = piece_c.x + 1;
    check_y = piece_c.y + player_move_multiplier;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
            Coord move_to{ piece_c.x + 1, piece_c.y + player_move_multiplier };

            if (check_cell->piece != nullptr) {
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin);
            }
            else {
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, false, check_for_pin);
            }
            // checks if the pawn can promote
            if (move_to.y == (board.move_number + 1) % 2 * 7) {
                // adds possible promotions of pawn
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin, b);
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin, n);
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin, r);
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin, q);
            }
            else {
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin);
            }
        }
    }

    // take piece to the left
    check_x = piece_c.x - 1;
    check_y = piece_c.y + player_move_multiplier;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
            move_to.x = check_x;
            move_to.y = check_y;

            // checks if the pawn can promote
            if (move_to.y == (board.move_number + 1) % 2 * 7) {
                // adds possible promotions of pawn
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin, b);
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin, n);
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin, r);
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin, q);
            }
            else {
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin);
            }
        }
    }

    if (board.move_number > 0) {
        PastMove last_move = board.past_moves.back();

        // en passant to the left
        check_x = piece_c.x - 1;
        check_y = piece_c.y;
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner
                && check_cell->piece->piece_type == p && last_move.to == check_cell->coord && last_move.from.y - (player_move_multiplier * 2) == check_y) {
                move_to.x = check_x;
                move_to.y = check_y + player_move_multiplier;
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin);
            }
        }

        // en passant to the right
        check_x = piece_c.x + 1;
        check_y = piece_c.y;    
        if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
            check_cell = &board.board[check_x][check_y];
            if (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner
                && check_cell->piece->piece_type == p && last_move.to == check_cell->coord && last_move.from.y - (player_move_multiplier * 2) == check_y) {
                move_to.x = check_x;
                move_to.y = check_y + player_move_multiplier;
                board.add_move(move_to, piece_c, check_cell->coord, player_move_multiplier, true, check_for_pin);
            }
        }
    }
}
