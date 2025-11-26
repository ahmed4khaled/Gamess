#pragma once
#include "BoardGame_Classes.h"
#include <string>
#include <vector>

class FiveByFive_UI : public UI<char> {
public:
    FiveByFive_UI();
    virtual ~FiveByFive_UI() = default;

    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;
private:
// int read_int_in_range(int minv, int maxv) const;

    // Helpers for AI
    int count_three_for_matrix(const std::vector<std::vector<char>>& mat, char symbol) const;

};
