#include "King.h"


void King::add_moves(Board& board, int player_move_multiplier, bool check_for_pin, BoardCell& board_cell) {
    Coord piece_c = this->piece_coord;
    Coord move_to{};
    int check_x;
    int check_y;

    BoardCell* check_cell{ nullptr };
    // checks to the top of the king for possible moves
    check_x = piece_c.x;
    check_y = piece_c.y + 1;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner) {
        if (board.board[check_x][check_y].piece == nullptr || board.board[check_x][check_y].piece != nullptr && board.board[check_x][check_y].piece->owner != board_cell.piece->owner)
            move_to.x = check_x;
            move_to.y = check_y;
            if (check_cell->piece != nullptr) {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
            }
            else {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
            }
        }
    }

    // checks to the bottom of the king for possible moves
    check_x = piece_c.x;
    check_y = piece_c.y - 1;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner)) {
            move_to.x = check_x;
            move_to.y = check_y;
            if (check_cell->piece != nullptr) {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
            }
            else {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
            }
        }
    }

    // checks to the left of the king for possible moves
    check_x = piece_c.x - 1;
    check_y = piece_c.y;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner)) {
            move_to.x = check_x;
            move_to.y = check_y;
            if (check_cell->piece != nullptr) {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
            }
            else {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
            }
        }
    }

    // checks to the right of the king for possible moves
    check_x = piece_c.x + 1;
    check_y = piece_c.y;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner)) {
            move_to.x = check_x;
            move_to.y = check_y;
            if (check_cell->piece != nullptr) {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
            }
            else {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
            }
        }
    }

    // checks to the top right of the king for possible moves
    check_x = piece_c.x + 1;
    check_y = piece_c.y + 1;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner)) {
            move_to.x = check_x;
            move_to.y = check_y;
            if (check_cell->piece != nullptr) {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
            }
            else {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
            }
        }
    }

    // checks to the top left of the king for possible moves
    check_x = piece_c.x - 1;
    check_y = piece_c.y + 1;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner)) {
            move_to.x = check_x;
            move_to.y = check_y;
            if (check_cell->piece != nullptr) {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
            }
            else {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
            }
        }
    }

    // checks to the bottom right of the king for possible moves
    check_x = piece_c.x + 1;
    check_y = piece_c.y - 1;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner)) {
            move_to.x = check_x;
            move_to.y = check_y;
            if (check_cell->piece != nullptr) {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
            }
            else {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
            }
        }
    }

    // checks to the bottom left of the king for possible moves
    check_x = piece_c.x - 1;
    check_y = piece_c.y - 1;
    if (check_x > -1 && check_x < board_size && check_y > -1 && check_y < board_size) {
        check_cell = &board.board[check_x][check_y];
        if (check_cell->piece == nullptr || (check_cell->piece != nullptr && check_cell->piece->owner != board_cell.piece->owner)) {
            move_to.x = check_x;
            move_to.y = check_y;
            if (check_cell->piece != nullptr) {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, true, check_for_pin);
            }
            else {
                board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin);
            }
        }
    }
    ++board.move_number;
    if (check_for_pin and !board.in_check() && piece_c.x == 4 && piece_c.y == this->owner*7) {
        --board.move_number;
        // for castling
        bool king_moved{ false };
        for (auto& past_move : board.past_moves) {
            if (past_move.from == board.board[4][piece_c.y].coord) {
                king_moved = true;
            }
        }

        if (!king_moved) {
            move_to.y = piece_c.y;

            // checks for short castle
            bool short_r_moved{ false };
            for (auto& past_move : board.past_moves) {
                if (past_move.from == board.board[7][piece_c.y].coord) {
                    short_r_moved = true;
                }
            }
            if (board.board[7][piece_c.y].piece == nullptr || board.board[7][piece_c.y].piece->piece_type != 500) { short_r_moved = true; }

            // checks for long castle
            bool long_r_moved{ false };
            for (auto& past_move : board.past_moves) {
                if (past_move.from == board.board[0][piece_c.y].coord) {
                    long_r_moved = true;
                }
            }
            if (board.board[0][piece_c.y].piece == nullptr || board.board[0][piece_c.y].piece->piece_type != 500) { long_r_moved = true; }

            // short castle
            if (!short_r_moved) {
                bool can_castle = true;

                check_cell = &board.board[piece_c.x + 1][piece_c.y];
                if (check_cell->piece == nullptr) {
                    move_to.x = piece_c.x + 1;
                    Move possible_move{ piece_c, move_to };
                    if (!board.is_move_legal(possible_move).is_legal) {
                        can_castle = false;
                    }
                } else{ can_castle = false; }

                check_cell = &board.board[piece_c.x + 2][piece_c.y];
                if (check_cell->piece == nullptr) {
                    move_to.x = piece_c.x + 2;
                    Move possible_move{ piece_c, move_to };
                    if (!board.is_move_legal(possible_move).is_legal) {
                        can_castle = false;
                    }
                } else{ can_castle = false; }

                if (can_castle) {
                    Coord move_to{ piece_c.x + 2, piece_c.y };

                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin, 0, true);
                }
            }

            // long castle
            if (!long_r_moved) {
                bool can_castle = true;

                check_cell = &board.board[piece_c.x - 1][piece_c.y];
                if (check_cell->piece == nullptr) {
                    move_to.x = piece_c.x - 1;
                    Move possible_move{ piece_c, move_to };
                    if (!board.is_move_legal(possible_move).is_legal) {
                        can_castle = false;
                    }
                } else{ can_castle = false; }

                check_cell = &board.board[piece_c.x - 2][piece_c.y];
                if (check_cell->piece == nullptr) {
                    move_to.x = piece_c.x - 2;
                    Move possible_move{ piece_c, move_to };
                    if (!board.is_move_legal(possible_move).is_legal) {
                        can_castle = false;
                    }
                } else{ can_castle = false; }

                check_cell = &board.board[1][piece_c.y];
                if (check_cell->piece != nullptr) { can_castle = false; }

                if (can_castle) {
                    move_to.x = piece_c.x - 2;
                    board.add_move(move_to, piece_c, move_to, player_move_multiplier, false, check_for_pin, 0, true);
                }
            }

        }
    }
    else {
        --board.move_number;
    }
}