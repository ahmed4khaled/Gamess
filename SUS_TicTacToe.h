#ifndef SUS_TICTACTOE_H
#define SUS_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <algorithm>

/**
 * @brief Board class for SUS Tic-Tac-Toe game
 *
 * A 3x3 grid where players place 'S' or 'U' to form "S-U-S" sequences.
 * Scoring is based on counting all S-U-S patterns (horizontal, vertical, diagonal).
 */
class SUS_Board : public Board<char> {
private:
    int player1_score;
    int player2_score;

    /**
     * @brief Count all S-U-S sequences on the board
     * @return Pair of scores (player1_score, player2_score)
     */
    std::pair<int, int> count_all_sus_sequences();

    /**
     * @brief Check if a specific line contains S-U-S pattern
     * @param c1 First character
     * @param c2 Second character
     * @param c3 Third character
     * @return true if sequence is S-U-S
     */
    bool is_sus_sequence(char c1, char c2, char c3);

public:
    SUS_Board();
    bool update_board(int x, int y, char symbol) override;
    void display_board() override;
    bool is_win() override;
    bool is_draw() override;
    bool game_is_over() override;

    /**
     * @brief Get current scores for both players
     * @return Pair of (player1_score, player2_score)
     */
    std::pair<int, int> get_scores();

    /**
     * @brief Undo a move (for backtracking)
     */
    void undo_move(int x, int y);
};

/**
 * @brief Player class for SUS Tic-Tac-Toe
 */
class SUS_Player : public Player<char> {
public:
    SUS_Player(std::string n, char symbol);
    void getmove(int& x, int& y) override;
};

/**
 * @brief AI Player using Minimax with backtracking
 */
class SUS_AI_Player : public Player<char> {
private:
    /**
     * @brief Minimax algorithm with alpha-beta pruning
     * @param board Current board state
     * @param depth Current depth in search tree
     * @param is_maximizing True if maximizing player's turn
     * @param alpha Alpha value for pruning
     * @param beta Beta value for pruning
     * @return Best score achievable
     */
    int minimax(SUS_Board* board, int depth, bool is_maximizing, int alpha, int beta);

    /**
     * @brief Find the best move using minimax
     * @param board Current board
     * @return Pair of (x, y) coordinates for best move
     */
    std::pair<int, int> find_best_move(SUS_Board* board);

    /**
     * @brief Evaluate board state (score difference)
     */
    int evaluate_board(SUS_Board* board);

public:
    SUS_AI_Player(char symbol);
    void getmove(int& x, int& y) override;
};

/**
 * @brief UI class for SUS Tic-Tac-Toe
 */
class SUS_UI : public UI<char> {
public:
    SUS_UI();
    Player<char>** setup_players() override;
    void display_winner(Player<char>* winner) override;

    /**
     * @brief Display current scores
     */
    void display_scores(std::pair<int, int> scores,
        Player<char>* player1,
        Player<char>* player2);
};

#endif // SUS_TICTACTOE_H