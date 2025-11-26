#pragma once
#include "BoardGame_Classes.h"
#include <string>
#include <unordered_set>

class Word_Board : public Board<char> {
public:
    // path: relative path to dic.txt (e.g. "dic.txt")
    Word_Board(const std::string& dic_path = "dic.txt", char blankSymbol = '-');
    virtual ~Word_Board() = default;

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

private:
    std::unordered_set<std::string> dict;
    bool last_move_won = false;
    int last_r = -1, last_c = -1;
    char blank;
    void load_dict(const std::string& path);
    bool check_three_at(int r, int c) const; // checks all lines length 3 passing through (r,c)
    bool full_board() const;
};
