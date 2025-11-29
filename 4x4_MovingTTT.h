#ifndef MOVING_TTT_4X4_H
#define MOVING_TTT_4X4_H

#include "BoardGame_Classes.h"
#include <iostream>
using namespace std;

class MovingTTT4x4_Board : public Board<char> {
public:
    MovingTTT4x4_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override { return false; }
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override { return is_win(p) || is_draw(p); }

private:
    bool in_bounds(int r, int c);
    bool check_three_in_row(char sym);
};

/////////////////////////////////////////////////////

class MovingTTT4x4_UI : public UI<char> {
public:
    MovingTTT4x4_UI() : UI<char>(3) {}

    Move<char>* get_move(Player<char>* player) override;
};

#endif
