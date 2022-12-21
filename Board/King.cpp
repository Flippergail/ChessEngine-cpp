#include "King.h"


void King::add_moves(Board& board, int player_move_multiplier, bool check_for_pin, BoardCell& board_cell) {
    Coord piece_c = this->piece_coord;
    int check_x = piece_c.x;
    int check_y = piece_c.y + player_move_multiplier;

    BoardCell* check_cell{ nullptr };
    // checks to the top of the king for possible moves
    check_x = piece_c.x;
    check_y = piece_c.y + 1;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
            Coord move_to{ check_x, check_y };
            board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
        }
    }

    // checks to the bottom of the king for possible moves
    check_x = piece_c.x;
    check_y = piece_c.y - 1;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
            Coord move_to{ check_x, check_y };
            board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
        }
    }

    // checks to the left of the king for possible moves
    check_x = piece_c.x - 1;
    check_y = piece_c.y;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
            Coord move_to{ check_x, check_y };
            board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
        }
    }

    // checks to the right of the king for possible moves
    check_x = piece_c.x + 1;
    check_y = piece_c.y;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
            Coord move_to{ check_x, check_y };
            board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
        }
    }

    // checks to the top right of the king for possible moves
    check_x = piece_c.x + 1;
    check_y = piece_c.y + 1;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
            Coord move_to{ check_x, check_y };
            board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
        }
    }

    // checks to the top left of the king for possible moves
    check_x = piece_c.x - 1;
    check_y = piece_c.y + 1;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
            Coord move_to{ check_x, check_y };
            board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
        }
    }

    // checks to the bottom right of the king for possible moves
    check_x = piece_c.x + 1;
    check_y = piece_c.y - 1;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
            Coord move_to{ check_x, check_y };
            board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
        }
    }

    // checks to the bottom left of the king for possible moves
    check_x = piece_c.x - 1;
    check_y = piece_c.y - 1;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
            Coord move_to{ check_x, check_y };
            board.add_move(move_to, piece_c, player_move_multiplier, check_for_pin);
        }

    }
    ++board.move_number;
    if (check_for_pin and board.in_check()) {
        --board.move_number;
        // for castling
        bool king_moved{ false };
        for (auto& past_move : board.past_moves) {
            if (past_move.from == board.board[4][(board.move_number % 2) * 7].coord) {
                king_moved = true;
            }
        }

        if (!king_moved) {

            // checks for short castle
            bool short_r_moved{ false };
            for (auto& past_move : board.past_moves) {
                if (past_move.from == board.board[7][(board.move_number % 2) * 7].coord) {
                    short_r_moved = true;
                }
            }

            // checks for long castle
            bool long_r_moved{ false };
            for (auto& past_move : board.past_moves) {
                if (past_move.from == board.board[0][(board.move_number % 2) * 7].coord) {
                    long_r_moved = true;
                }
            }

            // short castle
            if (!short_r_moved) {
                check_cell = &board.board[piece_c.x + 1][piece_c.y];
                if (check_cell->piece == nullptr) {
                    Coord move_to{ piece_c.x + 1, piece_c.y };
                    Move possible_move{ piece_c, move_to };
                    if (board.is_move_legal(possible_move)) {
                        ++move_to.x;
                        possible_move.to = move_to;
                        board.add_move(move_to, piece_c, player_move_multiplier, true, 0, true);
                    }
                }
            }

            // long castle
            if (!long_r_moved) {
                check_cell = &board.board[piece_c.x - 1][piece_c.y];
                if (check_cell->piece == nullptr && board.board[1][piece_c.y].piece == nullptr) {
                    Coord move_to{ piece_c.x - 1, piece_c.y };
                    Move possible_move{ piece_c, move_to };
                    if (board.is_move_legal(possible_move)) {
                        --move_to.x;
                        possible_move.to = move_to;
                        board.add_move(move_to, piece_c, player_move_multiplier, true, 0, true);
                    }
                }
            }

        }
    }
    else {
        --board.move_number;
    }
}