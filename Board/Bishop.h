#pragma once
#include "Piece.h"
#include "Board.h"

class Bishop : public Piece{
public:
    Bishop(const int piece_type_param, const int owner_param, Coord& coord_param) : Piece(piece_type_param, owner_param, coord_param) {}
    virtual ~Bishop() {};

    void add_moves(Board& board, int player_move_multiplier, bool check_for_pin, BoardCell& board_cell);
};

