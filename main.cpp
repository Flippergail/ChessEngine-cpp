#include <chrono>
#include <thread>

/*
code for waiting
std::chrono::seconds dura(5);
std::this_thread::sleep_for(dura);
*/


#include <iostream>
#include "board.h"

using namespace std;

void player_make_move(Board* board) {
    short int coord_from_x;
    short int coord_from_y;

    cin >> coord_from_x;
    if (coord_from_x == -1) {
        // undo last move
        board->undo_move();
        std::cout << *board << endl;
        player_make_move(board);

        return;
    }

    cin >> coord_from_y;

    short int coord_to_x;
    short int coord_to_y;

    cin >> coord_to_x;
    cin >> coord_to_y;

    Coord coord_from{ coord_from_x, coord_from_y };
    Coord coord_to{ coord_to_x, coord_to_y };

    Move my_move{ coord_from, coord_to };

    bool is_legal = board->make_move(my_move);
    if (is_legal) {
        std::cout << *board << endl;

        //for (auto &value : board->possible_moves) {
        //    std::cout << "[" << value.from.x << ", " << value.from.y<< "]" << endl;    
        //    std::cout << "[" << value.to.x << ", " << value.to.y<< "]" << endl;
        //    std::cout << "==========" << endl << endl;
        //}
    }
    else {
        std::cout << " move invalid " << endl;
        player_make_move(board);
    }
}


int main() {
    Board* board = new Board;
    std::cout << *board << endl;

    while (true)
    {
        player_make_move(board);
    }

    //board->undo_move();
    //std::cout << *board << endl;

    //std::cout << std::boolalpha;
    //std::cout << is_legal << std::endl;

    //for (auto &value : board->possible_moves) {
    //    std::cout << "[" << value.from.x << ", " << value.from.y<< "]" << endl;    
    //    std::cout << "[" << value.to.x << ", " << value.to.y<< "]" << endl;
    //    std::cout << "==========" << endl << endl;
    //}

    return 0;
}
