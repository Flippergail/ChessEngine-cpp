#pragma once
class Piece;

const int board_size{ 8 };

class Coord {
public:
	int x;
	int y; 
	
	bool operator==(const Coord& rhs) const {
		return x == rhs.x && y == rhs.y;
	}

};

class Move {
public:
	Coord from;
	Coord to;

	Piece* piece_taken;

	int promotion_to{};
	bool is_castle;

	bool causes_check{ false };

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

	int piece_taken;
	Coord piece_taken_coord;
	int piece_taken_owner;

	int piece_owner;
	int promoted_to;
	bool was_castle;
	int promoted_from;
};