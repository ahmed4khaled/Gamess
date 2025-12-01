#ifndef ULTIMATETTT_H
#define ULTIMATETTT_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

/**
 * @class UltimateTTTBoard
 * @brief Implements the logic for Ultimate Tic-Tac-Toe.
 *
 * The board consists of:
 * - A 9×9 grid.
 * - Divided into 3×3 sub-boards (each 3×3).
 * - Players win a sub-board using normal XO rules.
 * - Winning sub-boards populate a 3×3 "main board".
 * - Winning the main board (3-in-a-row of sub-boards) wins the game.
 *
 * Also enforces “forced board rules”:
 * - Your opponent's move determines the sub-board you must play in next.
 * - If that sub-board is closed (won/tied), you may play anywhere legal.
 */
class UltimateTTTBoard : public Board<char> {
private:
    /**
     * @brief Stores the winner of each of the 3×3 small boards.
     *
     * Values:
     * - 'X' → X won the sub-board
     * - 'O' → O won the sub-board
     * - 'T' → tie (full but no winner)
     * - '-' → still open
     */
    std::vector<std::vector<char>> mainBoardWinners;

    /**
     * @brief The next required sub-board the player must play in.
     * 
     * Values:
     * - (r,c) where r,c ∈ {0,1,2}
     * - (-1,-1) means “free move”: any open sub-board allowed.
     */
    std::pair<int,int> nextRequired;

    /**
     * @brief Determines if a specific sub-board (3×3) is finished.
     */
    bool isSmallClosed(int br, int bc) const;

    /**
     * @brief Evaluates and returns the winner of a sub-board.
     *
     * @return 'X', 'O', 'T' or '-'
     */
    char checkSmallWinner(int br, int bc) const;

public:
    /**
     * @brief Constructs the full 9×9 Ultimate Tic-Tac-Toe board.
     */
    UltimateTTTBoard();

    /**
     * @brief Checks if a full-board cell is legal to play.
     */
    bool is_cell_allowed(int full_r, int full_c) const;

    /**
     * @brief Returns the next required sub-board (or -1,-1).
     */
    std::pair<int,int> get_next_required() const;

    /**
     * @brief Returns the 9×9 matrix for display/UI.
     */
    std::vector<std::vector<char>> get_board_matrix() const;

    /**
     * @brief Updates the board with a move and adjusts nextRequired accordingly.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the player has won the main board.
     */
    bool is_win(Player<char>* p) override;

    /**
     * @brief Checks if the player has lost (opponent has main-board win).
     */
    bool is_lose(Player<char>* p) override;

    /**
     * @brief Checks if the entire game is a draw.
     */
    bool is_draw(Player<char>* p) override;

    /**
     * @brief Determines if the game has ended.
     */
    bool game_is_over(Player<char>* p) override;

    /**
     * @brief Checks if a symbol has 3-in-a-row on the main 3×3 board.
     */
    bool checkMainWin(char s);
};

/**
 * @class UltimateTTT_UI
 * @brief User interface for Ultimate Tic-Tac-Toe.
 *
 * Provides:
 * - Input handling
 * - Random AI selection
 * - UI messages on forced board locations
 */
class UltimateTTT_UI : public UI<char> {
public:
    /**
     * @brief Constructs UI with proper zoom factor.
     */
    UltimateTTT_UI();

    /**
     * @brief Retrieves a move from human or computer player.
     */
    Move<char>* get_move(Player<char>* p) override;
};

#endif // ULTIMATETTT_H
