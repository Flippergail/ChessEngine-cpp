#pragma once
#include "Move.h"

const int p = 100;
const int b = 305;
const int n = 300;
const int r = 500;
const int q = 900;
const int k = 2000;

class Board;
class BoardCell;

class Piece
{
public:
	Piece(const int piece_type_param, const int owner_param, Coord& coord_param);
	virtual ~Piece() {};

	//board_cell.piece->add_moves(*this, player_move_multiplier, check_for_pin);
	virtual void add_moves(Board& board, int player_move_multiplier, bool check_for_pin, BoardCell& board_cell){}
	Coord piece_coord{};
	int owner{};
	int piece_type{};
};