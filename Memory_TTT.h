/**
 * @file Memory_TTT.h
 * @brief Memory Tic-Tac-Toe (header). Board + UI declarations.
 *
 * Game idea:
 *  - 3x3 board.
 *  - When a player places a mark it is stored internally but displayed as HIDDEN.
 *  - Win if you have three of your marks aligned (the internals determine win).
 */

#ifndef MEMORY_TTT_H
#define MEMORY_TTT_H

#include "BoardGame_Classes.h"
#include <vector>

/**
 * @class MemoryBoard
 * @brief Board for Memory Tic-Tac-Toe: stores real symbols internally.
 */
class MemoryBoard : public Board<char> {
public:
    /// Constructor (initializes 3x3 internal board)
    MemoryBoard();

    /// Return internal board matrix (real symbols).
    std::vector<std::vector<char>> get_board_matrix() const;

    /**
     * @brief Apply move to the board.
     * @param mv pointer to Move<char> (uses mv->get_x()/get_y()/get_symbol()).
     * @return true if move applied, false otherwise.
     */
    bool update_board(Move<char>* mv) override;

    /**
     * @brief Check win for the given player.
     * Signature matches framework: takes Player<char>*.
     */
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;  
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

private:
    std::vector<std::vector<char>> real_board; ///< real stored symbols ('X','O' or '.')
    int moves_count;
};

/**
 * @class Memory_UI
 * @brief UI for Memory Tic-Tac-Toe. Displays hidden cells to users.
 *
 * Also contains a Minimax AI for COMPUTER players (good fit for 3x3).
 */
class Memory_UI : public UI<char> {
public:
    Memory_UI();

    /// Display the board (but showing hidden marks).
    void display_board_matrix(const std::vector<std::vector<char>>& mat) const override;

    /// Get next move from human or the AI (computer).
    Move<char>* get_move(Player<char>* player) override;

private:
    static int read_int_in_range(int minv, int maxv);
};

#endif // MEMORY_TTT_H
