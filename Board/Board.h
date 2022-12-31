#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "Piece.h"
#include "Move.h"

// game states
const int playing_state{ 2 };
const int black_win{ 1 };
const int draw{ -1 };
const int white_win{ 0 };

struct legal_and_check {
    bool is_legal, causes_check;
};

typedef struct legal_and_check Legal_And_Check_Struct;

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

    void add_move(Coord& move_to, Coord& piece_c, Coord piece_taken, int player_move_multiplier, bool is_taking_piece, bool check_for_pin = false, int promotion_to = 0, bool is_castle = false);
    void find_moves(bool check_for_pin = true);

    Legal_And_Check_Struct is_move_legal(Move& check_move);
    bool in_check();

    int game_state{ playing_state };
    bool currently_in_check{ false };

    int pushes{};
    int pops{};

    void print_metrics();

    std::vector<PastMove> past_moves{};
    std::vector<Move> possible_moves;
    std::vector<std::vector<Move>> past_possible_moves{};
    std::vector<Move> pin_moves;

    std::vector<int> white_pieces;
    std::vector<int> black_pieces;


private:
    void new_piece(BoardCell& board_cell, int piece_type, int piece_owner);
    bool are_only_pieces(int white_piece, int black_piece);
};