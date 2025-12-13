#pragma once
#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

/**
 * @class MisereBoard
 * @brief Misère Tic-Tac-Toe board (making 3-in-a-row = LOSE)
 */
class MisereBoard : public Board<char> {
public:
    MisereBoard();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // ===== AI helper functions (SAFE OOP) =====
    void make_move(int r, int c, char s);
    void undo_move(int r, int c);
    std::vector<std::pair<int,int>> get_available_moves() const;

    bool check_three(char s) const;
    bool full_board() const;

private:
    char blank = '-';
};

/**
 * @class MisereAIPlayer
 * @brief Minimax AI for Misère Tic-Tac-Toe
 */
class MisereAIPlayer : public Player<char> {
private:
    int minimax(MisereBoard* board, bool isMaximizing);

public:
    MisereAIPlayer(std::string name, char symbol);
    Move<char>* get_best_move();
};

/**
 * @class MisereUI
 * @brief UI for Misère Tic-Tac-Toe
 */
class MisereUI : public UI<char> {
public:
    MisereUI();

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;
    void display_rules() const;

private:
    int read_int_in_range(int minv, int maxv) const;
};
