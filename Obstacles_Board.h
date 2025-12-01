#pragma once
#include "BoardGame_Classes.h"
#include <vector>

/**
 * @class Obstacles_Board
 * @brief Tic-Tac-Toe with obstacles placed on a 5×5 grid.
 *
 * Game Rules:
 * - Board size: 5×5
 * - Empty cells = blank symbol
 * - Obstacles = obstacle symbol (cannot be overwritten)
 * - Player wins by forming **3-in-a-row** horizontally, vertically, or diagonally.
 * - Obstacles block lines and block placement.
 * - Draw: board full with no winner.
 *
 * Important:
 * - Obstacles are pre-inserted randomly or fixed depending on UI logic.
 * - Players place X or O in valid (non-obstacle, empty) cells.
 */
class Obstacles_Board : public Board<char> {
public:
    /**
     * @brief Creates a 5×5 board initialized with all blanks.
     */
    Obstacles_Board(char blankSymbol = '-', char obstacleSymbol = '#');

    /**
     * @brief Places symbol if cell is empty & not obstacle.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Win: current player has at least one 3-in-a-row.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Lose: opponent formed 3-in-a-row.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Draw: board is full and no win exists.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Ends if win or draw.
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Inserts obstacles at given coordinates.
     */
    void place_obstacles(const std::vector<std::pair<int,int>>& cells);

private:
    char blank;
    char obstacle;

    /**
     * @brief Checks whether symbol s has a 3-in-a-row.
     */
    bool check_win_symbol(char s) const;

    /**
     * @brief True if board contains no blanks.
     */
    bool full_board() const;
};
