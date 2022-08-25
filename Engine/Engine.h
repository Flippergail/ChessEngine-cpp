#pragma once

#include <iostream>
#include <vector>

#include "../Board/Board.h"

class Engine {
public:
	void make_move(Board& board);
	void evaluate_board(Board& board);
};