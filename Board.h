#pragma once

#include <iostream>
#include <vector>
#include "Piece.h"
#include "Move.h"

// game states
const short int playing_state{ 2 };
const short int black_win{ 1 };
const short int draw{ -1 };
const short int white_win{ 0 };

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
    unsigned short int move_number{ 0 };

    //Piece* board[8][8] = { nullptr };

    BoardCell board[8][8]{};

    Board();
    ~Board();

    //Setters
    void pop_move(bool check_moves = true);
    bool push_move(Move& player_move, bool check_for_pin = true);

    void find_moves(bool check_for_pin = true);

    short int game_state{ playing_state };

private:
    void add_move(Coord& move_to, Coord& piece_c, short int player_move_multiplier, bool check_for_pin = true, Piece* piece_taken = nullptr, bool is_castle = false);
    bool is_move_legal(Move& check_move);
    bool in_check();

    std::vector<PastMove> past_moves{};
    std::vector<Move> possible_moves{};
    std::vector<Move> pin_moves{};
};