#pragma once
#include "Move.h"

const short int p = 100;
const short int b = 305;
const short int n = 300;
const short int r = 500;
const short int q = 900;
const short int k = 2000;

class Board;
class BoardCell;

class Piece
{
public:
	Piece(const short int piece_type_param, const short int owner_param, Coord& coord_param);
	~Piece();

	//board_cell.piece->add_moves(*this, player_move_multiplier, check_for_pin);
	virtual void add_moves(Board& board, short int player_move_multiplier, bool check_for_pin, BoardCell& board_cell){}
	Coord piece_coord{};
	short int owner{};
	short int piece_type{};
};