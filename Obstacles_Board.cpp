#include "Obstacles_Board.h"
#include <iostream>

/* ============================================================
 *  CONSTRUCTOR
 * ============================================================ */

Obstacles_Board::Obstacles_Board(char blankSymbol, char obstacleSymbol)
    : Board<char>(5,5), blank(blankSymbol), obstacle(obstacleSymbol)
{
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            board[r][c] = blank;

    n_moves = 0;
}

/* ============================================================
 *  UPDATE BOARD
 * ============================================================ */

bool Obstacles_Board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char sym = move->get_symbol();

    if (r < 0 || r >= 5 || c < 0 || c >= 5)
        return false;

    if (board[r][c] != blank) // obstacle or filled
        return false;

    board[r][c] = sym;
    n_moves++;

    return true;
}

/* ============================================================
 *  WIN CHECK — 3-in-a-row
 * ============================================================ */

bool Obstacles_Board::check_win_symbol(char s) const {
    // horizontal
    for (int r = 0; r < 5; r++)
        for (int c = 0; c <= 2; c++)
            if (board[r][c] == s && board[r][c+1] == s && board[r][c+2] == s)
                return true;

    // vertical
    for (int c = 0; c < 5; c++)
        for (int r = 0; r <= 2; r++)
            if (board[r][c] == s && board[r+1][c] == s && board[r+2][c] == s)
                return true;

    // diag ↘
    for (int r = 0; r <= 2; r++)
        for (int c = 0; c <= 2; c++)
            if (board[r][c] == s && board[r+1][c+1] == s && board[r+2][c+2] == s)
                return true;

    // diag ↙
    for (int r = 0; r <= 2; r++)
        for (int c = 2; c < 5; c++)
            if (board[r][c] == s && board[r+1][c-1] == s && board[r+2][c-2] == s)
                return true;

    return false;
}

/* ============================================================
 *  FULL BOARD?
 * ============================================================ */

bool Obstacles_Board::full_board() const {
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            if (board[r][c] == blank)
                return false;
    return true;
}

/* ============================================================
 *  WIN / LOSE / DRAW
 * ============================================================ */

bool Obstacles_Board::is_win(Player<char>* player) {
    return check_win_symbol(player->get_symbol());
}

bool Obstacles_Board::is_lose(Player<char>* player) {
    char opp = (player->get_symbol() == 'X' ? 'O' : 'X');
    return check_win_symbol(opp);
}

bool Obstacles_Board::is_draw(Player<char>* /*player*/) {
    return full_board() &&
           !check_win_symbol('X') &&
           !check_win_symbol('O');
}

bool Obstacles_Board::game_is_over(Player<char>* p) {
    return is_win(p) || is_draw(p) || is_lose(p);
}

/* ============================================================
 *  OBSTACLE INSERTION
 * ============================================================ */

void Obstacles_Board::place_obstacles(const std::vector<std::pair<int,int>>& cells) {
    for (auto& pr : cells) {
        int r = pr.first;
        int c = pr.second;
        if (r >= 0 && r < 5 && c >= 0 && c < 5)
            board[r][c] = obstacle;
    }
}
