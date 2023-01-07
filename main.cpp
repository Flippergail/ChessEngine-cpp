#include <iostream>
#include <string>

#include "Board/Board.h"
#include "Engine/Engine.h"

using namespace std;


void player_push_move(Board* board) {
    cout << *board << endl;

    int coord_from_x;
    int coord_from_y;


    int coord_to_x;
    int coord_to_y;

    string typed_move;
    cout << "Please enter your move : ";
    getline(cin, typed_move);

    if (typed_move == "-1") {
        // undo last move
        board->pop_move();
        board->pop_move();
        cout << *board << endl;
        player_push_move(board);
        return;
    }

    coord_from_x = static_cast<int>(typed_move[0]) - 97;
    coord_from_y = static_cast<int>(typed_move[1])-48-1;

    coord_to_x = static_cast<int>(typed_move[2]) - 97;
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
        cout << *board << endl;
    }
    else {
        cout << " move invalid " << endl;
        player_push_move(board);
    }
}

bool check_game_state(Board* board) {
    if (board->game_state == 1) {
        cout << "Black has won!" << endl;
    }
    else if (board->game_state == 0) {
        cout << "White has won!" << endl;
    }
    else if (board->game_state == -1) {
        cout << "It is a draw!" << endl;
    }

    if (board->game_state != 2) { return false; }
    else { return true; }
}


int main() {
    string matchFormat;
    cout << "Would you like to play the engine or watch a game of the engine vs engine (type 1 or 2) : ";
    getline(cin, matchFormat);

    Board* board = new Board;

    if (matchFormat == "1") {
        string playerColour;
        cout << "Would you like to play white, black or random? w/b/r : ";
        getline(cin, playerColour);

        Engine* engine = new Engine;


        if (playerColour == "r") {
            //srand(time(0));
            int randomint = rand() % 3;
            if (randomint == 2) {
                playerColour = "w"; cout << "You are playing as White." << endl;
                engine->engineColour = 1;
            }
            else {
                playerColour = "b"; cout << "You are playing as Black." << endl;
                engine->engineColour = 0;
            }
        }
        bool game_playing{ true };

        while (game_playing)
        {
            if (playerColour == "w") {
                player_push_move(board);

                game_playing = check_game_state(board);
                if (!game_playing) { break; }

                engine->make_move(*board);
                game_playing = check_game_state(board);
            }
            else {
                engine->make_move(*board);

                game_playing = check_game_state(board);
                if (!game_playing) { break; }

                player_push_move(board);
                game_playing = check_game_state(board);
            }
        }
    }
    else if (matchFormat == "2") {

        Engine* engine_white = new Engine;
        engine_white->engineColour = 0;
        Engine* engine_black = new Engine;
        engine_black->engineColour = 1;

        bool game_playing{ true };
        while (game_playing) {
            engine_white->make_move(*board);
            cout << *board << endl;

            game_playing = check_game_state(board);
            if (!game_playing) { break; }

            engine_black->make_move(*board);
            cout << *board << endl;

            game_playing = check_game_state(board);
        }
    }
    else if (matchFormat == "3") {

        bool game_playing{ true };
        while (game_playing) {
            player_push_move(board);
            game_playing = check_game_state(board);
        }
    }
    

    return 0;
}
