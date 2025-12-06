/**
 * @file InfinityTicTacToe.h
 * @brief Infinity Tic-Tac-Toe game declarations (3x3 board, oldest move removed after every 3 moves).
 * 
 *  A game on a 3x3 grid where the oldest moves are periodically removed from the board.
 *  After every three moves, the oldest mark on the board disappears.
 *  The first player to align three marks in a row before any of those marks vanish wins the game.
 
 */

#ifndef _INFINITY_TICTACTOE_H
#define _INFINITY_TICTACTOE_H

#include "BoardGame_classes.h"
#include <queue>
#include <utility>
#include <string>

 /**
 *  @brief Infinity game board : 3x3 with rolling(oldest removed) behavior.
 */
template <typename T>
class InfinityBoard : public Board<T> {
private:
    /**
    * @brief Queue to track move chronology for removal.
    */
    queue<pair<int, int>> history_X;  
    queue<pair<int, int>> history_O;  
    /**
     * @brief Number of moves after which the oldest move is removed.
	 */
    int moves_before_removal = 3;                

public:
    /**
     * @brief Construct an empty 3x3 Infinity board.
     */
    InfinityBoard();

    /**
     * @brief Place a move on the board (if valid) and manage history.
     * @param move pointer to Move<T>
     * @return true on success, false otherwise
     */
    bool update_board(Move<T>* move) override;

    /**
     * @brief Check if given player has won (three aligned).
     * @param player pointer to player
     * @return true if player has 3 in a row
     */
    bool is_win(Player<T>* player) override;

    /**
     * @brief Not used; loss is determined by opponent's win.
     */
    bool is_lose(Player<T>* player) override;

    /**
     * @brief Detect draw (safety limit to avoid infinite games).
     * @return true if n_moves exceeded safe limit
     */
    bool is_draw(Player<T>* player) override;

    /**
     * @brief Combined check if game over for player.
     */
    bool game_is_over(Player<T>* player) override;
};

/**
 * @brief Player class for Infinity Tic-Tac-Toe.
 */
template <typename T>
class InfinityPlayer : public Player<T> {
public:
    /**
     * @brief Construct player.
     * @param name player name
     * @param symbol player symbol
     * @param type player type
     */
    InfinityPlayer(std::string name, T symbol, PlayerType type);
};

/**
 * @brief UI (console) for Infinity Tic-Tac-Toe.
 *
 * Provides get_move (human or random AI) and create_player override to produce InfinityPlayer.
 */
template <typename T>
class InfinityUI : public UI<T> {
public:
    /**
     * @brief Construct UI and show short welcome.
     */
    InfinityUI();

    /**
     * @brief Ask player for move (human input) or generate random valid move for computer.
     * @param player pointer to player
     * @return Move<T>* (newed; caller/manager is responsible for deletion if needed)
     */
    Move<T>* get_move(Player<T>* player) override;

    /**
     * @brief Create a new InfinityPlayer instance.
     */
    Player<T>* create_player(std::string& name, T symbol, PlayerType type) override;
};

#endif 