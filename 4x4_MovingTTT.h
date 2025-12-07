#ifndef MOVING_TTT_4X4_H
#define MOVING_TTT_4X4_H

#include "BoardGame_Classes.h"
#include <iostream>
using namespace std;

/**
 * @class MovingTTT4x4_Board
 * @brief Implements a 4x4 Moving Tic-Tac-Toe board.
 *
 * In this variant, pieces can be moved after the board becomes full.
 * The board inherits from Board<char> and overrides all required logic.
 */
class MovingTTT4x4_Board : public Board<char> {
public:

    /**
     * @brief Constructs a 4x4 Moving Tic-Tac-Toe board.
     */
    MovingTTT4x4_Board();

    /**
     * @brief Applies a move (placing or moving a symbol).
     * @param move Pointer to the Move<char> containing row, col, and symbol.
     * @return true if the move is legal and was applied, false otherwise.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if a player has won.
     * @param p Pointer to player whose win status is being checked.
     * @return true if the player has won.
     */
    bool is_win(Player<char>* p) override;

    /**
     * @brief Checks if a player has lost (always false for this variant).
     * @param p Pointer to the player.
     * @return Always false, as losing is not explicitly defined.
     */
    bool is_lose(Player<char>* p) override { return false; }

    /**
     * @brief Checks if the game ended in a draw.
     * @param p Pointer to player.
     * @return true if draw condition is met.
     */
    bool is_draw(Player<char>* p) override;

    /**
     * @brief Determines if the game is over.
     * @param p Pointer to the player.
     * @return true if win or draw conditions are met.
     */
    bool game_is_over(Player<char>* p) override { return is_win(p) || is_draw(p); }

private:
    /**
     * @brief Validates if a given cell lies within the 4x4 grid.
     * @param r Row index
     * @param c Column index
     * @return true if (r,c) is inside bounds.
     */
    bool in_bounds(int r, int c);

    /**
     * @brief Checks if the player has three of their symbols in a row.
     * @param sym Symbol belonging to the player.
     * @return true if a 3-in-a-row exists.
     */
    bool check_three_in_row(char sym);
};

/////////////////////////////////////////////////////

/**
 * @class MovingTTT4x4_UI
 * @brief Handles user input/output for the 4x4 Moving Tic-Tac-Toe game.
 */
class MovingTTT4x4_UI : public UI<char> {
public:

    /**
     * @brief Constructs the UI handler with board size = 3 for display scaling.
     */
    MovingTTT4x4_UI();
    
    /**
     * @brief Virtual destructor
     */
    virtual ~MovingTTT4x4_UI();

    /**
     * @brief Gets a move from a player (human or computer).
     * @param player Pointer to the requesting player.
     * @return Pointer to Move<char> containing row, column, and symbol.
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif
