/**
 * @file Connect4.cpp
 * @brief Implementation of Connect Four game.
 */

#include "Connect4.h"
#include <cstdlib>

 /**
  * @section board_section Board Implementation
  * This section contains all board-related logic for the Connect Four game.
  */

Connect4_Board::Connect4_Board() : Board<char>(6, 7) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            board[i][j] = ' ';
}

bool Connect4_Board::update_board(Move<char>* move) {
    int col = move->get_y();
    if (col < 0 || col >= columns) return false;

    for (int r = rows - 1; r >= 0; --r) {
        if (board[r][col] == ' ') {
            board[r][col] = move->get_symbol();
            n_moves++;
            return true;
        }
    }
    return false;
}

bool Connect4_Board::check_dir(int r, int c, int dr, int dc, char s) {
    for (int k = 0; k < 4; ++k) {
        int nr = r + dr * k;
        int nc = c + dc * k;
        if (nr < 0 || nr >= rows || nc < 0 || nc >= columns)
            return false;
        if (board[nr][nc] != s)
            return false;
    }
    return true;
}

bool Connect4_Board::is_win(Player<char>* p) {
    char s = p->get_symbol();

    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            if (check_dir(r, c, 0, 1, s) ||
                check_dir(r, c, 1, 0, s) ||
                check_dir(r, c, 1, 1, s) ||
                check_dir(r, c, 1, -1, s))
                return true;

    return false;
}

bool Connect4_Board::is_lose(Player<char>* p) {
    return false;
}

bool Connect4_Board::is_draw(Player<char>* p) {
    return n_moves == rows * columns;
}

bool Connect4_Board::game_is_over(Player<char>* p) {
    return is_win(p) || is_draw(p);
}

/**
 * @section Smart AI Implementation
 * This section contains all AI-related logics for the Connect Four game.
 */

SmartAI_Player::SmartAI_Player(std::string name, char symbol)
    : Player<char>(name, symbol, PlayerType::AI) {}

int SmartAI_Player::evaluate() {
    auto mat = boardPtr->get_board_matrix();
    int score = 0;

    for (int r = 0; r < 6; ++r)
        for (int c = 0; c < 7; ++c) {
            if (mat[r][c] == symbol) score += 3;
            else if (mat[r][c] != ' ') score -= 2;
        }
    return score;
}

Move<char>* SmartAI_Player::get_move() {
    int bestCol = 0;
    int bestScore = -100000;

    for (int c = 0; c < 7; ++c) {
        for (int r = 5; r >= 0; --r) {
            if (boardPtr->get_cell(r, c) == ' ') {

                int val = rand() % 100 + evaluate();

                if (val > bestScore) {
                    bestScore = val;
                    bestCol = c;
                }
                break;
            }
        }
    }
    return new Move<char>(0, bestCol, symbol);
}

/**
 * @section UI Implementation
 * This section contains user interface for the Connect Four game.
 */
Move<char>* Connect4_UI::get_move(Player<char>* p) {
    int col;

    if (p->get_type() == PlayerType::HUMAN) {
        cout << p->get_name() << " choose column (0-6): ";
        cin >> col;
        return new Move<char>(0, col, p->get_symbol());
    }
    else {
        SmartAI_Player* ai = (SmartAI_Player*)p;
        return ai->get_move();
    }
}
