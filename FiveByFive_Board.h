#pragma once
#include "BoardGame_Classes.h"
#include <vector>

class FiveByFive_Board : public Board<char> {
public:
    FiveByFive_Board(); 
    virtual ~FiveByFive_Board() = default;

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // helper: return board matrix (Board<T> already has get_board_matrix() but
    // other code may prefer direct access)
    // Note: Board<T>::board is protected, but easier to use get_board_matrix()
private:
    char blank = '.';



    

    int count_three_lines(char symbol) const;

    bool full_except_one() const;
};
