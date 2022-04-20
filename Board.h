#pragma once

#include <iostream>
#include <vector>
#include "Piece.h"
#include "Move.h"

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

    std::vector<Move> possible_moves;

    Board();
    ~Board();

    //Setters
    void undo_move();
    bool make_move(Move& player_move, bool check_for_pin = true);

    std::vector<PastMove> past_moves;
    void find_moves(bool check_for_pin = true);

private:
    void add_move(Coord& move_to, Coord& piece_c, short int player_move_multiplier, bool check_for_pin = true, Piece* piece_taken = nullptr);
    bool is_pinned(Move& pin_move);
    std::vector<Move> pin_moves;
};