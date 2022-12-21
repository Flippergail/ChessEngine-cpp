#include "Piece.h"
#include <iostream>
#include "Board.h"

Piece::Piece(const int piece_type_param, const int owner_param, Coord& coord_param)
{
	owner = owner_param;
	piece_type = piece_type_param;
	piece_coord = coord_param;

}

Piece::~Piece()
{
	//std::cout << piece_type << " deconstructed, owner : " << owner << std::endl;
}