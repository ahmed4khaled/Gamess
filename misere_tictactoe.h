#ifndef MISERE_TICTACTOE_H
#define MISERE_TICTACTOE_H

#include "BoardGame_Classes.h"
using namespace std;

class MisereBoard : public Board<char> {
private:
    char blank_symbol = '.';

public:
    MisereBoard();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override { return false; } // not used
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    // helper
    bool has_three(char sym) const;
};

class MisereUI : public UI<char> {
public:
    MisereUI();

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;

private:
    // Minimax helpers
    int minimax(vector<vector<char>>& boardm, char turnSym, char maxSym, int depth);
    bool board_has_three(const vector<vector<char>>& boardm, char sym) const;
    pair<int,int> best_move_minimax(char mySym);
};

#endif // MISERE_TICTACTOE_H
