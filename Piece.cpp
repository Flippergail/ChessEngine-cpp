#include "Piece.h"
#include <iostream>

Piece::Piece(const short int piece_type_param, const short int owner_param, Coord& coord_param)
{
	owner = owner_param;
	piece_type = piece_type_param;
	piece_coord = coord_param;

	//std::cout << piece_type << " constructed, owner : " << owner << std::endl;
}

Piece::~Piece()
{
	std::cout << piece_type << " deconstructed, owner : " << owner << std::endl;
}