#pragma once
#include "BoardGame_Classes.h"
#include <string>
#include <vector>

class Obstacles_UI : public UI<char> {
public:
    Obstacles_UI();
    virtual ~Obstacles_UI() = default;

    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;

private:
    int read_int_in_range(int minv, int maxv) const;

    // AI helpers
    int count_four_for_matrix(const std::vector<std::vector<char>>& mat, char symbol) const;
    int evaluate_move_for_ai(std::vector<std::vector<char>>& mat, int r, int c, char mySym, char oppSym) const;
};
