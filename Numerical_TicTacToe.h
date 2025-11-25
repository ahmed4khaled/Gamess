/**
 * @file Numerical_TicTacToe.h
 * @brief Defines the Numerical Tic-Tac-Toe specific classes that extend the generic board game framework.
 *
 * This file provides:
 * - `Numerical_TicTacToe_Board`: A specialized board class for the Numerical Tic-Tac-Toe game.
 * - `Numerical_TicTacToe_UI`: A user interface class tailored to Numerical Tic-Tac-Toe game setup and player interaction.
 */

#ifndef NUMERICAL_TICTACTOE_H
#define NUMERICAL_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <set>
using namespace std;

/**
 * @class Numerical_TicTacToe_Board
 * @brief Represents the Numerical Tic-Tac-Toe game board.
 *
 * This class inherits from the generic `Board<int>` class and implements
 * the specific logic required for Numerical Tic-Tac-Toe, where players
 * use numbers (odd vs even) and aim to make a line sum to exactly 15.
 *
 * @see Board
 */
class Numerical_TicTacToe_Board : public Board<int> {
private:
    int blank_symbol = 0; ///< Value used to represent an empty cell on the board.
    set<int> player1_available; ///< Available odd numbers for Player 1: {1, 3, 5, 7, 9}
    set<int> player2_available; ///< Available even numbers for Player 2: {2, 4, 6, 8}

public:
    /**
     * @brief Default constructor that initializes a 3x3 Numerical Tic-Tac-Toe board.
     */
    Numerical_TicTacToe_Board();

    /**
     * @brief Updates the board with a player's move.
     * @param move Pointer to a Move<int> object containing move coordinates and number.
     * @return true if the move is valid and successfully applied, false otherwise.
     */
    bool update_board(Move<int>* move);

    /**
     * @brief Checks if the given player has won the game.
     * @param player Pointer to the player being checked.
     * @return true if the player has a line summing to exactly 15, false otherwise.
     */
    bool is_win(Player<int>* player);

    /**
     * @brief Checks if the given player has lost the game.
     * @param player Pointer to the player being checked.
     * @return Always returns false (not used in this game logic).
     */
    bool is_lose(Player<int>*) { return false; };

    /**
     * @brief Checks if the game has ended in a draw.
     * @param player Pointer to the player being checked.
     * @return true if all cells are filled and no line sums to 15, false otherwise.
     */
    bool is_draw(Player<int>* player);

    /**
     * @brief Determines if the game is over (win or draw).
     * @param player Pointer to the player to evaluate.
     * @return true if the game has ended, false otherwise.
     */
    bool game_is_over(Player<int>* player);

    /**
     * @brief Gets the set of available numbers for a player.
     * @param player_num Player number (1 for odd numbers, 2 for even numbers).
     * @return Reference to the set of available numbers.
     */
    const set<int>& get_available_numbers(int player_num) const;

    /**
     * @brief Checks if a cell is empty.
     * @param x Row index.
     * @param y Column index.
     * @return true if the cell is empty, false otherwise.
     */
    bool is_cell_empty(int x, int y) const;
};


/**
 * @class Numerical_TicTacToe_UI
 * @brief User Interface class for the Numerical Tic-Tac-Toe game.
 *
 * Inherits from the generic `UI<int>` base class and provides
 * Numerical Tic-Tac-Toe specific functionality for player setup and move input.
 *
 * @see UI
 */
class Numerical_TicTacToe_UI : public UI<int> {
public:
    /**
     * @brief Constructs a Numerical_TicTacToe_UI object.
     *
     * Initializes the base `UI<int>` class with the welcome message.
     */
    Numerical_TicTacToe_UI();

    /**
     * @brief Destructor for Numerical_TicTacToe_UI.
     */
    ~Numerical_TicTacToe_UI() {};

    /**
     * @brief Creates a player of the specified type.
     * @param name Name of the player.
     * @param symbol Number identifier (1 for odd player, 2 for even player).
     * @param type The type of the player (Human or Computer).
     * @return Pointer to the newly created Player<int> instance.
     */
    Player<int>* create_player(string& name, int symbol, PlayerType type);

    /**
     * @brief Retrieves the next move from a player.
     * @param player Pointer to the player whose move is being requested.
     * @return A pointer to a new `Move<int>` object representing the player's action.
     */
    Move<int>* get_move(Player<int>* player) override;

    /**
     * @brief Sets up the players for the game.
     * @return Array of pointers to the two Player<int> instances.
     */
    Player<int>** setup_players() override;
};

#endif // NUMERICAL_TICTACTOE_H