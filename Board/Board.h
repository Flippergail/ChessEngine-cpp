#pragma once

#include <iostream>
#include <vector>
#include "Piece.h"
#include "Move.h"

// game states
const int playing_state{ 2 };
const int black_win{ 1 };
const int draw{ -1 };
const int white_win{ 0 };

class BoardCell
{
public:
    Coord coord;
    Piece* piece;
};

class Board
{
    friend std::ostream& operator<<(std::ostream& out, const Board& board_class);
public:
    unsigned int move_number{ 0 };

    BoardCell board[8][8];

    Board();
    ~Board();

    void pop_move(bool check_moves = true);
    bool push_move(Move& player_move, bool check_for_pin = true);

    void add_move(Coord& move_to, Coord& piece_c, int player_move_multiplier, bool check_for_pin = true, Piece* piece_taken = nullptr, int promotion_to = 0, bool is_castle = false);
    void find_moves(bool check_for_pin = true);

    bool is_move_legal(Move& check_move);
    bool in_check();

    int game_state{ playing_state };

    int pushes{};
    int pops{};

    std::vector<PastMove> past_moves{};
    std::vector<Move> possible_moves;
    std::vector<Move> pin_moves;

    std::vector<int> white_pieces;
    std::vector<int> black_pieces;


private:
    void new_piece(BoardCell& board_cell, int piece_type, int piece_owner);
    bool are_only_pieces(int white_piece, int black_piece);
};