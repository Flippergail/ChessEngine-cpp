//#include <chrono>
//#include <thread>



#include <iostream>
#include <string>
#include "Board/Board.h"

using namespace std;


void player_push_move(Board* board) {
    short int coord_from_x;
    short int coord_from_y;


    short int coord_to_x;
    short int coord_to_y;

    std::string typed_move;
    //cin >> typed_move;
    getline(cin, typed_move);

    if (typed_move == "-1") {
        // undo last move
        board->pop_move();
        std::cout << *board << endl;
        player_push_move(board);
        return;
    }

    coord_from_x = static_cast<int>(typed_move[0]) - 97;
    coord_from_y = static_cast<int>(typed_move[1])-48-1;

    coord_to_x = static_cast<short int>(typed_move[2]) - 97;
    coord_to_y = static_cast<int>(typed_move[3])-48-1;


    Coord coord_from{ coord_from_x, coord_from_y };
    Coord coord_to{ coord_to_x, coord_to_y };

    Move my_move{ coord_from, coord_to };

    // checks if the player typed a promotion
    if (typed_move.length() == 6) {
        // promotes the piece
        if (typed_move[5] == 'b') {
            my_move.promotion_to = b;
        }
        else if (typed_move[5] == 'n') {
            my_move.promotion_to = n;
        }
        else if (typed_move[5] == 'r') {
            my_move.promotion_to = r;
        }
        else if (typed_move[5] == 'q') {
            my_move.promotion_to = q;
        }
    }

    bool is_legal = board->push_move(my_move);
    if (is_legal) {
        std::cout << *board << endl;
    }
    else {
        std::cout << " move invalid " << endl;
        player_push_move(board);
    }
}


int main() {
    Board* board = new Board;
    std::cout << *board << endl;

    while (true)
    {
        player_push_move(board);
    }

    return 0;
}
