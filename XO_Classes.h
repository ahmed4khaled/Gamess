#pragma once
#include "BoardGame_Classes.h"

/**
 * @class X_O_Board
 * @brief Standard 3×3 Tic-Tac-Toe board.
 *
 * Rules:
 * - Board size: 3×3
 * - Symbols: X or O
 * - A player wins by forming 3-in-a-row in any direction.
 * - Draw: board is full and no win.
 */
class X_O_Board : public Board<char> {
public:

    /**
     * @brief Initializes a 3×3 board filled with '-'.
     */
    X_O_Board();

    /**
     * @brief Places a symbol on the board if the move is valid.
     * @return true if move succeeded.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief True if the player has a 3-in-a-row.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Lose when opponent wins.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Draw: full board and no winner.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Game ends when win or draw.
     */
    bool game_is_over(Player<char>* player) override;

private:
    /// Character for empty cell.
    char blank = '-';

    /**
     * @brief Checks if 3-in-a-row exists for symbol s.
     */
    bool check_win(char s) const;

    /**
     * @brief Returns true if board contains no blank cells.
     */
    bool full_board() const;
};

/**
 * @class XO_UI
 * @brief UI for classic Tic-Tac-Toe.
 *
 * Responsibilities:
 * - Ask user for row/col.
 * - Simple AI: pick random empty cell.
 * - Display 3×3 grid.
 */
class XO_UI : public UI<char> {
public:

    XO_UI();
    virtual ~XO_UI() = default;

    /**
     * @brief Requests move from human or AI.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Creates human or computer player.
     */
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /**
     * @brief Prints the 3×3 board.
     */
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;

private:

    /**
     * @brief Reads integer safely in specified range.
     */
    int read_int_in_range(int minv, int maxv) const;
};
