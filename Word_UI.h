#pragma once
#include "BoardGame_Classes.h"
#include <string>

/**
 * @class Word_UI
 * @brief User Interface class for the Word Game (3×3 letter placement).
 *
 * Responsibilities:
 * - Reading moves from human players.
 * - Displaying the board in a readable format.
 * - Providing simple AI behavior:
 *     • Try immediate win.
 *     • Try blocking opponent’s immediate win.
 *     • Choose heuristic best move.
 */
class Word_UI : public UI<char> {
public:

    /**
     * @brief Initializes UI (seed randomizer).
     */
    Word_UI();

    virtual ~Word_UI() = default;

    /**
     * @brief Gets a move from human or computer player.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Creates either a human or AI player.
     */
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /**
     * @brief Displays the 3×3 letter board.
     */
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;

private:

    /**
     * @brief Reads an integer safely from minv to maxv.
     */
    int read_int_in_range(int minv, int maxv) const;

    /**
     * @brief Reads a single letter A–Z from input.
     */
    char read_letter() const;

    /**
     * @brief Checks if a given 3-letter word is valid based on loaded dictionary.
     */
    bool check_word_in_dict(const std::string& s) const;
};
