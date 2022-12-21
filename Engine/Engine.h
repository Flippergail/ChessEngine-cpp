#pragma once

#include <iostream>
#include <vector>

#include "../Board/Board.h"
#include "../Board/Move.h"

class Engine {
public:
	void make_move(Board& board);

private:
	const int treeDepth{ 2 };
	const int quiesceDepth{ 0 };

	Move bestMove{};
	int bestValue{ -999999 };

	int pushes{};
	int pops{};

	int alphabeta(Board& board, int alpha, int beta, int treeDepthLeft);
	int quiesce(Board& board, int alpha, int beta, int treeDepthLeft);
	int evaluatePosition(Board& board);
};