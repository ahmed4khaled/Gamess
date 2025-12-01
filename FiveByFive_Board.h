#pragma once
#include "BoardGame_Classes.h"
#include <vector>

/**
 * @class FiveByFive_Board
 * @brief Implements the logic for the 5x5 Tic-Tac-Toe variant.
 *
 * Players place X/O on a 5×5 grid.  
 * Game ends when *at most one* empty cell remains.  
 * Winner = player with the highest number of 3-in-a-row lines.
 */
class FiveByFive_Board : public Board<char> {
public:
    /**
     * @brief Constructs a 5×5 board initialized with '.'.
     */
    FiveByFive_Board();

    virtual ~FiveByFive_Board() = default;

    /**
     * @brief Attempts to update the board based on the move.
     * @param move Pointer to Move<char> containing coordinates + symbol.
     * @return true if the update succeeds.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks whether the given player has won.
     * @return true if player's 3-lines > opponent’s 3-lines once board is nearly full.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the player has lost.
     * @return true if opponent has more 3-lines.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if the game is a draw (equal number of 3-lines).
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Game ends when 0 or 1 empty cells remain.
     */
    bool game_is_over(Player<char>* player) override;

private:
    /// Symbol used to denote an empty cell.
    char blank = '.';

    /**
     * @brief Counts total 3-in-a-row sequences for a given symbol.
     */
    int count_three_lines(char symbol) const;

    /**
     * @brief Returns true if the board has ≤1 empty cell.
     */
    bool full_except_one() const;
};
