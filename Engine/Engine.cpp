#include "Engine.h"

void Engine::make_move(Board& board) {
	this->bestValue = -999999;
	int alpha{ -999999 };
	int beta{ 999999 };
	std::vector<Move> possible_moves = board.possible_moves;
	for (auto &move : possible_moves) {
		bool legal = board.push_move(move);
		if (legal == true) {
			int boardValue = -this->alphabeta(board, -beta, -alpha, this->treeDepth - 1);
			if (boardValue > this->bestValue) {
				this->bestValue = boardValue;
				this->bestMove = move;
			}
			if (boardValue > alpha) {
				alpha = boardValue;
			}
			board.pop_move();
		}
	}

	board.push_move(this->bestMove);
}

int Engine::alphabeta(Board& board, int alpha, int beta, int treeDepthLeft) {
	int max = -999999;
	if (treeDepthLeft == 0) { return this->quiesce(board, alpha, beta, this->quiesceDepth); }

	std::vector<Move> possible_moves = board.possible_moves;
	for (auto &move : possible_moves) {
		bool legal = board.push_move(move);
		if (legal == true) {
			int score = -this->alphabeta(board, -beta, -alpha, treeDepthLeft - 1);
			board.pop_move();

			if (score >= beta) { return score; }
			if (score > max) { max = score; }
			if (score > alpha) { alpha = score; }
		}
	}
	return max;
}

int Engine::quiesce(Board& board, int alpha, int beta, int treeDepthLeft) {
	int position_eval{ this->evaluatePosition(board) };
	if (position_eval >= beta) { return beta; }
	if (treeDepthLeft == 0) { return position_eval; }

	std::vector<Move> possible_moves = board.possible_moves;
	for (auto &move : possible_moves) {
		if (move.piece_taken != 0) {
			board.push_move(move);
			int score{ -this->quiesce(board, -beta, -alpha, treeDepthLeft - 1) };
			board.pop_move();

			if (score >= beta) { return beta; }
			if (score > alpha) { alpha = score; }
		}
	}
}

int Engine::evaluatePosition(Board& board) {
	srand(time(0));
	int randomint = rand() % 100;
	return randomint;
}