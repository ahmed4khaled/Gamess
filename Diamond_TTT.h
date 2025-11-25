#ifndef DIAMOND_TTT_H
#define DIAMOND_TTT_H

#include "BoardGame_Classes.h"
#include <vector>
using namespace std;

class DiamondBoard : public Board<char> {
public:
    DiamondBoard();
    bool is_valid_cell(int r, int c) const;
    bool update_board(Move<char>* move) override;
    vector<int> get_directions(Player<char>* player, int length) const;
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool game_is_over(Player<char>* player) override;
};

class DiamondUI : public UI<char> {
public:
    DiamondUI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif