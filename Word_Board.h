#pragma once
#include "BoardGame_Classes.h"
#include <string>
#include <unordered_set>

/**
 * @class Word_Board
 * @brief Implements a 3×3 word-forming game where players place letters.
 *
 * Game Rules:
 * - Board is 3×3 and starts empty ('-').
 * - A player places a letter (A–Z) each turn.
 * - After a placement, all lines passing through that cell (row, column,
 *   diag, anti-diag) are checked.
 * - If any line forms a *valid 3-letter word* from the dictionary,
 *   the current player immediately wins.
 * - If the board becomes full with no word formed, the game is a draw.
 *
 * Dictionary:
 * - Loaded from `dic.txt`.
 * - Only 3-letter uppercase words are accepted.
 */
class Word_Board : public Board<char> {
public:

    /**
     * @brief Initializes the board and loads dictionary from file.
     * @param dic_path Path to dictionary file (default "dic.txt").
     * @param blankSymbol Character that represents an empty cell.
     */
    Word_Board(const std::string& dic_path = "dic.txt", char blankSymbol = '-');

    virtual ~Word_Board() = default;

    /**
     * @brief Places a letter on the board and checks for word formation.
     * @return true if move is valid and applied.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief The player wins if their last move formed a valid word.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief There is no explicit lose condition in this game.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Draw: board full and no word was formed.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Game over when someone forms a word or when board is full.
     */
    bool game_is_over(Player<char>* player) override;

private:

    /// Dictionary of valid uppercase 3-letter words
    std::unordered_set<std::string> dict;

    /// Tracks if last move resulted in forming a valid word
    bool last_move_won = false;

    /// Last move position
    int last_r = -1, last_c = -1;

    /// Character used to represent an empty cell
    char blank;

    /**
     * @brief Loads and uppercases valid 3-letter words into dict.
     */
    void load_dict(const std::string& path);

    /**
     * @brief Checks if the row, column, diag, or anti-diag through (r,c)
     *        forms a valid word.
     */
    bool check_three_at(int r, int c) const;

    /**
     * @brief Checks if the entire board contains no empty cells.
     */
    bool full_board() const;
};
