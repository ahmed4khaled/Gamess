#pragma once
#include "BoardGame_Classes.h"
#include <string>


class Word_UI : public UI<char> {
public:
    Word_UI();
    virtual ~Word_UI() = default;

    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;

private:
    int read_int_in_range(int minv, int maxv) const;
    char read_letter() const;
        // AI helpers

        bool check_word_in_dict(const std::string& s) const;

};
