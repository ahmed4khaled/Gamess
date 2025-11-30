#ifndef DIAMOND_TTT_H
#define DIAMOND_TTT_H

#include "BoardGame_Classes.h"
#include <vector>
using namespace std;

/**
 * @class DiamondBoard
 * @brief Implements the board for Diamond Tic-Tac-Toe (5x5 diamond shape).
 *
 * Only cells that satisfy |r - 2| + |c - 2| <= 2 are playable.
 * Win condition: player must form **one 3-in-a-row AND one 4-in-a-row** 
 * in different directions.
 */
class DiamondBoard : public Board<char> {
public:
    /**
     * @brief Initializes the 5x5 diamond board.
     * Non-playable cells are marked with '#', playable with '.'.
     */
    DiamondBoard();

    /**
     * @brief Checks if a board cell belongs to the diamond shape.
     * @param r Row index.
     * @param c Column index.
     * @return true if the cell is playable.
     */
    bool is_valid_cell(int r, int c) const;

    /**
     * @brief Attempts to apply a move to board.
     * @param move Move pointer containing coordinates and symbol.
     * @return true if the move is legal and applied.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Returns list of directions in which the player has `length` consecutive symbols.
     * @param player Player whose symbol to check.
     * @param length Required sequence length (3 or 4).
     * @return Vector of integers (1=horizontal,2=vertical,3=diag↘,4=diag↙)
     */
    vector<int> get_directions(Player<char>* player, int length) const;

    /**
     * @brief Checks if a player has won.
     * @param player The player whose win condition is checked.
     * @return true if player has both a 3-in-a-row and 4-in-a-row in different directions.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks draw condition (board filled with 13 playable moves).
     * @param player Player pointer (unused).
     * @return true if board is full without a win.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Losing not defined for this game.
     * @return Always false.
     */
    bool is_lose(Player<char>* player) override { return false; }

    /**
     * @brief Determines if the game has ended.
     * @return true if win or draw.
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Returns all empty playable cells.
     * @return Vector of (row,col) pairs.
     */
    vector<pair<int, int>> get_available_moves() const;

    /**
     * @brief Checks if a given symbol has met the win state.
     * @param symbol Symbol to check.
     * @return true if both a 3-sequence and 4-sequence exist in different directions.
     */
    bool check_win_state(char symbol) const;

    /**
     * @brief Places symbol on board (used by AI minimax).
     */
    void make_move(int r, int c, char symbol);

    /**
     * @brief Removes symbol (undo a move).
     */
    void undo_move(int r, int c);

    /**
     * @brief Finds directions where given symbol has a sequence of length `length`.
     */
    vector<int> get_directions_for_symbol(char symbol, int length) const;
};

/**
 * @class DiamondAIPlayer
 * @brief AI player using minimax with alpha-beta pruning for Diamond Tic-Tac-Toe.
 */
class DiamondAIPlayer : public Player<char> {
private:
    static const int MAX_DEPTH = 6;

    /**
     * @brief Helper struct to store move evaluation results.
     */
    struct MoveScore {
        int row;
        int col;
        int score;
    };

    /**
     * @brief Minimax evaluation with alpha-beta pruning.
     * @return Integer score of the board state.
     */
    int minimax(DiamondBoard* board, int depth, bool isMaximizing,
                char aiSymbol, char humanSymbol, int alpha, int beta);

    /**
     * @brief Heuristic board evaluation used by minimax.
     */
    int evaluate_board(DiamondBoard* board, char aiSymbol, char humanSymbol);

public:
    /**
     * @brief Constructs an AI player.
     */
    DiamondAIPlayer(string name, char symbol);

    /**
     * @brief Computes and returns the best possible move.
     */
    Move<char>* get_best_move();
};

/**
 * @class DiamondUI
 * @brief Handles user input/output for the Diamond Tic-Tac-Toe game.
 */
class DiamondUI : public UI<char> {
public:
    /**
     * @brief Basic constructor with welcome message.
     */
    DiamondUI();

    /**
     * @brief Creates either a human or AI player depending on type.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Gets move from player (AI or human).
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif
