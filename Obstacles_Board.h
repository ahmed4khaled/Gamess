#pragma once
#include "BoardGame_Classes.h"
#include <vector>
#include <random>

class Obstacles_Board : public Board<char> {
public:
    Obstacles_Board(char blankSymbol = '.', char obstacleSymbol = '#');
    virtual ~Obstacles_Board() = default;

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // helpers
    std::vector<std::vector<char>> get_board_matrix() const;
    bool is_cell_free(int r, int c) const; // public check for UI

private:
    char obstacle;
    char blank;
    std::mt19937 rng;

    int rows_local() const { return rows; }
    int cols_local() const { return columns; }

    void add_random_obstacles(int count);
    bool place_single_random_obstacle();
    int count_four_in_row(char symbol) const;
    bool board_full_except_obstacles() const;

    // track last move for correct win attribution
    int last_r = -1, last_c = -1;
    char last_symbol = 0;
    bool last_move_created_win = false;
};
