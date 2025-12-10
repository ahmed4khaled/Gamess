#pragma once
#include "BoardGame_Classes.h"

/**
 * @class MisereBoard
 * @brief Implements Misère Tic-Tac-Toe (avoid making 3-in-a-row).
 *
 * Rules:
 * - Standard 3×3 grid.
 * - Players place X or O normally.
 * - The player who **forms** a 3-in-a-row **loses**, not wins.
 * - If the board becomes full without anyone forming 3-in-a-row,
 *   the game is a draw.
 */
class MisereBoard : public Board<char> {
public:
    /**
     * @brief Initializes a 3×3 board with all cells = '-'.
     */
    MisereBoard();

    /**
     * @brief Places a symbol on the board if the move is legal.
     * @return true if move was applied.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Player wins if the opponent just created 3-in-a-row.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Player loses if THEY create 3-in-a-row.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Draw if full board and no one loses.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Ends when someone loses or board is full.
     */
    bool game_is_over(Player<char>* player) override;

private:
    char blank = '-';

    /**
     * @brief Checks whether symbol has any 3-in-a-row.
     */
    bool check_three(char s) const;

    /**
     * @brief True if no blank cells remain.
     */
    bool full_board() const;
};

/**
 * @class MisereUI
 * @brief User interface for Misère Tic-Tac-Toe.
 *
 * Human Player:
 * - Enter row + col.
 *
 * Computer Player:
 * - Random valid move (safe if possible).
 */
class MisereUI : public UI<char> {
public:
    MisereUI();
    virtual ~MisereUI() = default;

    /**
     * @brief Reads user's or AI's move.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Creates a player object.
     */
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /**
     * @brief Displays 3×3 board.
     */
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;
    /**
     * @brief Displays Misère Tic-Tac-Toe rules.
     */
    void display_rules() const ;

private:
    /**
     * @brief Safely reads an integer between minv and maxv.
     */
    int read_int_in_range(int minv, int maxv) const;
};
