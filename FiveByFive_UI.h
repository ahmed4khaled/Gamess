#pragma once
#include "BoardGame_Classes.h"
#include <string>
#include <vector>

/**
 * @class FiveByFive_UI
 * @brief Handles all user interaction for the 5×5 Tic-Tac-Toe game.
 *
 * This UI collects player moves, prints the board, and creates players.
 * Also contains simple AI logic for computer players.
 */
class FiveByFive_UI : public UI<char> {
public:
    /**
     * @brief Constructs the UI with default board size 2 for display formatting.
     */
    FiveByFive_UI();

    virtual ~FiveByFive_UI() = default;

    /**
     * @brief Requests a move from the given player.
     * @param player Pointer to the current player.
     * @return Pointer to a Move<char> object.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Creates a player (human or computer).
     * @param name Name of the player.
     * @param symbol Player's symbol (X or O).
     * @param type HUMAN or COMPUTER.
     */
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /**
     * @brief Displays the current board state.
     */
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;

private:
    /**
     * @brief Counts the number of 3-in-a-row sequences for a symbol in a matrix.
     * @param mat Board matrix.
     * @param symbol X or O.
     * @return Number of 3-in-a-row matches.
     */
    int count_three_for_matrix(const std::vector<std::vector<char>>& mat, char symbol) const;
};
