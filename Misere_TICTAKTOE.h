#ifndef MISERE_TICTACTOE_H
#define MISERE_TICTACTOE_H

#include "BoardGame_Classes.h"

class MisereBoard : public Board<char> {
private:
    char blank_symbol = '.';

    bool has_three_in_row(char s) const;

public:
    MisereBoard();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;   
    bool is_lose(Player<char>* player) override;  
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};


class MisereUI : public UI<char> {
public:
    MisereUI();
    ~MisereUI() {};

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif
