#include "Engine.h"

void Engine::make_move(Board& board) {
	auto start = high_resolution_clock::now();

	Move bestMove{};
	int bestValue{ -999999 };

	int alpha{ -999999 };
	int beta{ 999999 };

	bool is_legal;
	int boardValue;
	std::vector<Move> possible_moves = board.possible_moves;

	PastMove last_move{};
	PastMove move_before_last{};
	if (board.move_number > 1) {
		last_move = board.past_moves.end()[-2];
	}
	//if (board.move_number > 3) {
	//	move_before_last = board.past_moves.end()[-4];
	//}

	if (possible_moves.size() > 1) {
		for (auto &move : possible_moves) {
			// makes sure the engine does not repeat moves
			if (move_repeating == true ||
				move.from != last_move.to || move.to != last_move.from) {

				total_branches += 1;
				is_legal = board.push_move(move);

				if (!is_legal) {
					std::cout << "error with move engine attempted to make (make_move)" << std::endl;
					std::cout << "updated move number : " << board.move_number << std::endl;
					std::cout << board << std::endl;
				}
				// if the game state is not the playing state (2) then it evaluates the current position, else continues deeper into the tree
				if (board.game_state != 2) {
					--board.move_number;
					boardValue = this->evaluatePosition(board);
					++board.move_number;
				}
				else { boardValue = -this->negamax(board, -beta, -alpha, treeDepth - 1); }

				if (boardValue > bestValue) {
					bestValue = boardValue;
					bestMove = move;
				}
				if (boardValue > alpha) {
					alpha = boardValue;
				}
				board.pop_move();
			}
		}
	}
	else {
		bestMove = possible_moves[0];
	}

	is_legal = board.push_move(bestMove);

	std::cout << "========================" << std::endl
		<< std::endl;\

	if (!is_legal) {
		std::cout << "error with move engine attempted to make (final make_move)" << std::endl;
		std::cout << "updated move number : " << board.move_number << std::endl;
 		std::cout << board << std::endl;
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	std::cout << "engine move duration : " << duration.count() << " seconds" << std::endl;
	std::cout << "move made value : " << bestValue << std::endl;
	std::cout << "tree leaves: " << tree_leaves << std::endl;
	std::cout << "total branches: " << total_branches << std::endl;
	std::cout << "updated move number : " << board.move_number << std::endl;

	tree_leaves = 0;
	total_branches = 0;
}

int Engine::negamax(Board& board, int alpha, int beta, int treeDepthLeft) {
	if (treeDepthLeft == 0) { return this->quiescence(board, alpha, beta, quiescenceDepth); }
	int max = -999999;
	bool is_legal;
	int score;

	std::vector<Move> possible_moves = board.possible_moves;
	for (auto &move : possible_moves) {
		total_branches += 1;
		is_legal = board.push_move(move);
		if (!is_legal) {
			std::cout << "error with move engine attempted to make (negamax)" << std::endl;
			std::cout << "updated move number : " << board.move_number << std::endl;
			std::cout << board << std::endl;
		}
		// if the game state is not the playing state (2) then it evaluates the current position, else continues deeper into the tree
		if (board.game_state != 2) {
			--board.move_number;
			score = this->evaluatePosition(board);
			++board.move_number;
		}
		else { score = -this->negamax(board, -beta, -alpha, treeDepthLeft - 1); }
		board.pop_move();

		if (score >= beta) { return score; }
		if (score > max) { 
			max = score;
			if (score > alpha) { alpha = score; }
		}
	}
	return max;
}


int Engine::quiescence(Board& board, int alpha, int beta, int treeDepthLeft) {
	int position_eval{ this->evaluatePosition(board) };

	// Delta Pruning
	if (position_eval >= beta) { return beta; }

	if (position_eval < alpha - BIG_DELTA) { return alpha; }
	if (alpha < position_eval) { alpha = position_eval; }

	if (treeDepthLeft == 0) { return position_eval; }

	bool is_legal;
	int score;

	std::vector<Move> possible_moves = board.possible_moves;
	for (auto &move : possible_moves) {
		if (move.piece_taken != 0 || move.causes_check || board.currently_in_check) {
			total_branches += 1;
			is_legal = board.push_move(move);
			if (!is_legal) {
				std::cout << "error with move engine attempted to make (quiescence)" << std::endl;
				std::cout << "updated move number : " << board.move_number << std::endl;
				std::cout << board << std::endl;
			}
			// if the game state is not the playing state (2) then it evaluates the current position, else continues deeper into the tree
			if (board.game_state != 2) {
				--board.move_number;
				score = this->evaluatePosition(board);
				++board.move_number;
			}
			else{ score = -this->quiescence(board, -beta, -alpha, treeDepthLeft - 1); }
			board.pop_move();

			if (score >= beta) { return beta; }
			if (score > alpha) { alpha = score; }
		}
	}
	return alpha;
}

int Engine::actual_weight(Board& board) {
	int engineActualWeight{};
	int oppositionActualWeight{};

	switch (board.move_number % 2)
	{
	case 0:
		for (auto piece : board.white_pieces) {
			switch (piece)
			{
			case 2000:
				break;
			default:
				engineActualWeight += piece;
				break;
			}
		}

		for (auto piece : board.black_pieces) {
			switch (piece)
			{
			case 2000:
				break;
			default:
				oppositionActualWeight += piece;
				break;
			}
		}
		break;

	case 1:
		for (auto piece : board.white_pieces) {
			switch (piece)
			{
			case 2000:
				break;
			default:
				oppositionActualWeight += piece;
				break;
			}
		}

		for (auto piece : board.black_pieces) {
			switch (piece)
			{
			case 2000:
				break;
			default:
				engineActualWeight += piece;
				break;
			}
		}
		break;

	default:
		break;
	}
	return (engineActualWeight - oppositionActualWeight);
}

int Engine::rough_position_weight(Board& board) {
	int score{};
	int scoreMult{};
	int owner{};
	for (int y = 0; y < board_size; ++y)
	{
		for (int x = 0; x < board_size; x++)
		{
			if (board.board[x][y].piece != nullptr) {
				owner = board.board[x][y].piece->owner;
				if (owner == board.move_number % 2) { scoreMult = 1; }
				else { scoreMult = -1; }

				switch (board.board[x][y].piece->piece_type)
				{
				case p :
					// pawn
					if (owner == 0){
						score += (scoreMult)*white_pawn_table[x][y];
					}
					else {
						score += (scoreMult)*black_pawn_table[x][y];
					}
					break;
				case b :
					// bishop
					if (owner == 0) {
						score += (scoreMult)*white_bishops_table[x][y];
					}
					else {
						score += (scoreMult)*black_bishops_table[x][y];
					}
					break;
				case n :
					// knight
					if (owner == 0) {
						score += (scoreMult)*white_knights_table[x][y];
					}
					else {
						score += (scoreMult)*black_knights_table[x][y];
					}
					break;
				case r :
					// rook
					if (owner == 0) {
						score += (scoreMult)*white_rooks_table[x][y];
					}
					else {
						score += (scoreMult)*black_rooks_table[x][y];
					}
					break;
				case q :
					// queen
					if (owner == 0) {
						score += (scoreMult)*white_queens_table[x][y];
					}
					else {
						score += (scoreMult)*black_queens_table[x][y];
					}
					break;
				case k :
					// king
					if (owner == 0) {
						score += (scoreMult)*white_kings_table[x][y];
					}
					else {
						score += (scoreMult)*black_kings_table[x][y];
					}
					break;
				default:
					break;
				}
			}
		}
	}
	return score;
}

int Engine::evaluatePosition(Board& board) {
	tree_leaves += 1;
	int score{};
	if (board.game_state == 2) {
		// game playing
		score += this->actual_weight(board);
		score += this->rough_position_weight(board);
		return score;

	} else if (board.game_state == board.move_number%2) {
		// current player on tree won by checkmate
		return 999999;

	} else if (board.game_state == -1) {
		// draw
		return 0;

	} else {
		// current player on tree won by checkmate
		return -999999;
	}
}