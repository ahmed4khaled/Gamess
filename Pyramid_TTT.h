/**
 * @file Pyramid_TTT.h
 * @brief Header for Pyramid Tic-Tac-Toe (Board + UI declarations).
 */

#ifndef PYRAMID_TTT_H
#define PYRAMID_TTT_H

#include "BoardGame_Classes.h"
#include <vector>

/**
 * PyramidBoard: 3x5 matrix representation (invalid cells '#', empty '.').
 */
class PyramidBoard : public Board<char> {
public:
    PyramidBoard(); // will call Board<char>(3,5)

    std::vector<std::vector<char>> get_board_matrix() const;

    bool is_valid_cell(int r, int c) const;

    // override signatures MUST match framework (take Player<char>* where required)
    bool update_board(Move<char>* mv) override;

    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

private:
    // do NOT duplicate 'board' or 'n_moves' here because they are inherited (protected).
    // helper (optional)
    bool inside_bounds(int r, int c) const;
};

/**
 * Pyramid_UI: UI for Pyramid game
 */
class Pyramid_UI : public UI<char> {
public:
    Pyramid_UI();
    Move<char>* get_move(Player<char>* player) override;
    void display_board_matrix(const std::vector<std::vector<char>>& mat) const override;

private:
    Move<char>* ask_human(Player<char>* player);
    static int read_int_in_range(int minv, int maxv);
};

#endif // PYRAMID_TTT_H
