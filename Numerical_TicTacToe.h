#ifndef NUMERICAL_TICTACTOE_H
#define NUMERICAL_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <set>

/**
 * @class Numerical_TicTacToe_Board
 * @brief Implements Numerical Tic-Tac-Toe game logic.
 *
 * Rules:
 * - Board is 3×3.
 * - Player 1 uses ODD numbers: {1,3,5,7,9}
 * - Player 2 uses EVEN numbers: {2,4,6,8}
 * - A number cannot be reused.
 * - Win: any row/column/diagonal summing to EXACTLY 15.
 */
class Numerical_TicTacToe_Board : public Board<int> {
public:

    /**
     * @brief Constructs the board and initializes available numbers.
     */
    Numerical_TicTacToe_Board();

    /**
     * @brief Places a number if valid.
     * @return true if move applied.
     */
    bool update_board(Move<int>* move) override;

    /**
     * @brief Checks if ANY line sums to 15.
     */
    bool is_win(Player<int>* player) override;
bool is_lose(Player<int>* player) override;

    /**
     * @brief Draw: board full & no win.
     */
    bool is_draw(Player<int>* player) override;

    /**
     * @brief Ends on win or draw.
     */
    bool game_is_over(Player<int>* player) override;

    /**
     * @brief Returns available numbers for a given player.
     * @param player_num 1 → odd, 2 → even
     */
    const std::set<int>& get_available_numbers(int player_num) const;

    /**
     * @brief Checks if a cell is empty.
     */
    bool is_cell_empty(int x, int y) const;

private:
    /// Cell value representing empty
    int blank_symbol = 0;

    /// Available numbers for odd player
    std::set<int> player1_available;

    /// Available numbers for even player
    std::set<int> player2_available;
};

/**
 * @class Numerical_TicTacToe_UI
 * @brief Handles input and AI for Numerical Tic-Tac-Toe.
 */
class Numerical_TicTacToe_UI : public UI<int> {
public:
    Numerical_TicTacToe_UI();

    /**
     * @brief Creates a player (odd/even).
     */
    Player<int>* create_player(std::string& name, int symbol, PlayerType type) override;

    /**
     * @brief Gets move from human or AI.
     */
    Move<int>* get_move(Player<int>* player) override;
    /**
     * @brief Displays Numerical Tic-Tac-Toe rules.
     */
    void display_rules() const ;
};

#endif
