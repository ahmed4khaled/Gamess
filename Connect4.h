#ifndef CONNECT4_H
#define CONNECT4_H

#include "BoardGame_classes.h"   
#include <string>

/**
 * @class Connect4_Board
 * @brief Represents a 6x7 Connect Four board.
 */
class Connect4_Board : public Board<char> {
public:
    
    /**
     * @brief Construct a new Connect4 Board.
     */
    Connect4_Board();
    
    /**
    * @brief Apply a move to the board.
    * @param move Pointer to the move.
    * @return true if move is valid.
    */
    bool update_board(Move<char>* move) override;
    
    /**
     * @brief Check if player has won.
     */
    bool is_win(Player<char>* p) override;
    
    /**
     * @brief Check if player has lost.
     */
    bool is_lose(Player<char>* p) override;
    
    /**
     * @brief Check for draw state.
     */
    bool is_draw(Player<char>* p) override;
    
    /**
     * @brief Check if game is over.
     */
    bool game_is_over(Player<char>* p) override;

private:
    /**
     * @brief Check 4 connected cells in one direction.
     * @param r Row start
     * @param c Column start
     * @param dr Row delta
     * @param dc Column delta
     * @param s Symbol to check
     */
    bool check_dir(int r, int c, int dr, int dc, char s);
};

/**
 * @class SmartAI_Player
 * @brief Simple AI player for Connect Four.
 */
class SmartAI_Player : public Player<char> {
public:

    /**
     * @brief Construct AI player.
     */
    SmartAI_Player(std::string name, char symbol);

     /**
     * @brief Generate AI move.
     */
    Move<char>* get_move();

private:
    
    /**
     * @brief Evaluate board score from AI perspective.
     * @return Score integer
     */
    int evaluate();
};

/**
 * @class Connect4_UI
 * @brief User interface for Connect Four.
 */
class Connect4_UI : public UI<char> {
public:   

    /**
     * @brief Ask player for a move.
     */
    Move<char>* get_move(Player<char>* p) override;
};

#endif
