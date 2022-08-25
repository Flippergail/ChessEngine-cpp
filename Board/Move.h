#pragma once
class Piece;

const short int board_size{ 8 };

class Coord {
public:
	short int x;
	short int y; 
	
	bool operator==(const Coord& rhs) const {
		return x == rhs.x && y == rhs.y;
	}

};

class Move {
public:
	Coord from;
	Coord to;

	Piece* piece_taken;

	short int promotion_to{};
	bool is_castle;

	bool operator==(const Move& rhs) const {
		return from.x == rhs.from.x && from.y == rhs.from.y
			&& to.x == rhs.to.x && to.y == rhs.to.y && promotion_to == rhs.promotion_to;
	}

};

class PastMove
{
public:
	Coord from;
	Coord to;

	short int piece_taken;
	Coord piece_taken_coord;

	short int promoted_to;
	bool was_castle;
	short int promoted_from;
};