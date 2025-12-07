/**
 * @file SUS.h
 * @brief SUS Game Board and UI Declarations
 */

#pragma once
#include "BoardGame_Classes.h"
#include <map>
 /**
  * @class SUSBoard
  * @brief 3x3 board for SUS game with scoring system.
  */
class SUSBoard : public Board<char> {
private:
    int score[2];   ///< Scores of both players
    /**
     * @brief Check if last move created a "S-U-S" sequence.
     */
    void check_sus(int x, int y, char s);

public:
    /// Constructor
    SUSBoard();

    /// Update board with a move
    bool update_board(Move<char>* move) override;

    /// Get score of player (0 or 1)
    int get_score(int i);

    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override;
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;
};

/**
 * @class SUS_UI
 * @brief User Interface for SUS game.
 */
class SUS_UI : public UI<char> {
private:
    /**
     * @brief Get Fixed letter for each player
     */
private:
    std::map<Player<char>*, char> playerLetter; 

    char ask_letter(Player<char>* p);
    Move<char>* smart_computer_move(Player<char>* p);


public:
    /// Constructor
    SUS_UI();
    
    /// Virtual destructor
    virtual ~SUS_UI();
    
    /// Get move from current player
    Move<char>* get_move(Player<char>* p) override;
};