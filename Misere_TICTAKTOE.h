#ifndef MISERE_TICTACTOE_H
#define MISERE_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <string>

class MisereBoard : public Board<char> {
private:
    char blank_symbol = '.';

    // helper: check whether symbol s has any 3-in-a-row on the board
    bool has_three_in_row(char s) const;

public:
    MisereBoard();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;   // For Misère we return false (no instant win)
    bool is_lose(Player<char>* player) override;  // True if player has 3-in-a-row (they lose)
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};


class MisereUI : public UI<char> {
public:
    MisereUI();
    ~MisereUI() {};

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;

private:
    // Backtracking/minimax helper used only when computer is playing.
    // Returns evaluation score from viewpoint of 'aiSymbol'.
    int minimax(MisereBoard* board,
                char aiSymbol,
                char curSymbol,
                int depth,
                bool isMax);

    // Helper to find best move for aiSymbol; returns newly allocated Move<char>
    Move<char>* find_best_move(MisereBoard* board, char aiSymbol);
};

#endif // MISERE_TICTACTOE_H
