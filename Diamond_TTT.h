#ifndef DIAMOND_TTT_H
#define DIAMOND_TTT_H

#include "BoardGame_Classes.h"
#include <vector>
using namespace std;

class DiamondBoard : public Board<char> {
public:
    DiamondBoard();
    bool is_valid_cell(int r, int c) const;
    bool update_board(Move<char>* move) override;
    vector<int> get_directions(Player<char>* player, int length) const;
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool game_is_over(Player<char>* player) override;
    
    // Helper methods for AI
    vector<pair<int, int>> get_available_moves() const;
    bool check_win_state(char symbol) const;
    void make_move(int r, int c, char symbol);
    void undo_move(int r, int c);
    vector<int> get_directions_for_symbol(char symbol, int length) const;
};

class DiamondAIPlayer : public Player<char> {
private:
    static const int MAX_DEPTH = 6;
    
    struct MoveScore {
        int row;
        int col;
        int score;
    };
    
    int minimax(DiamondBoard* board, int depth, bool isMaximizing, char aiSymbol, char humanSymbol, int alpha, int beta);
    int evaluate_board(DiamondBoard* board, char aiSymbol, char humanSymbol);
    
public:
    DiamondAIPlayer(string name, char symbol);
    Move<char>* get_best_move();
};

class DiamondUI : public UI<char> {
public:
    DiamondUI();
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif