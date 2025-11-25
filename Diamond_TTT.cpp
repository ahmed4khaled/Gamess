#include "BoardGame_Classes.h"
#include "Diamond_TTT.h"
#include <iostream>
#include <limits>
#include <cctype>
using namespace std;

bool DiamondBoard::is_valid_cell(int r, int c) const {
    return (abs(r - 2) + abs(c - 2) <= 2);
}

DiamondBoard::DiamondBoard() : Board<char>(5, 5) {
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            board[r][c] = is_valid_cell(r, c) ? '.' : '#';

    n_moves = 0;
}

bool DiamondBoard::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char sym = toupper(move->get_symbol());

    if (r < 0 || r >= 5 || c < 0 || c >= 5) return false;
    if (!is_valid_cell(r, c)) return false;
    if (board[r][c] != '.') return false;

    board[r][c] = sym;
    n_moves++;
    return true;
}

/*
 Directions:
 1 = horizontal
 2 = vertical
 3 = diag ↘
 4 = diag ↙
*/

vector<int> DiamondBoard::get_directions(Player<char>* player, int length) const {
    char S = player->get_symbol();
    vector<int> dirs;

    // horizontal
    for (int r = 0; r < 5; r++)
        for (int c = 0; c <= 5 - length; c++) {
            bool ok = true;
            for (int k = 0; k < length; k++)
                if (!is_valid_cell(r, c+k) || board[r][c+k] != S) ok = false;
            if (ok) dirs.push_back(1);
        }

    // vertical
    for (int c = 0; c < 5; c++)
        for (int r = 0; r <= 5 - length; r++) {
            bool ok = true;
            for (int k = 0; k < length; k++)
                if (!is_valid_cell(r+k, c) || board[r+k][c] != S) ok = false;
            if (ok) dirs.push_back(2);
        }

    // diag ↘
    for (int r = 0; r <= 5 - length; r++)
        for (int c = 0; c <= 5 - length; c++) {
            bool ok = true;
            for (int k = 0; k < length; k++)
                if (!is_valid_cell(r+k, c+k) || board[r+k][c+k] != S) ok = false;
            if (ok) dirs.push_back(3);
        }

    // diag ↙
    for (int r = 0; r <= 5 - length; r++)
        for (int c = length-1; c < 5; c++) {
            bool ok = true;
            for (int k = 0; k < length; k++)
                if (!is_valid_cell(r+k, c-k) || board[r+k][c-k] != S) ok = false;
            if (ok) dirs.push_back(4);
        }

    return dirs;
}

bool DiamondBoard::is_win(Player<char>* player) {
    if (!player) return false;

    vector<int> d3 = get_directions(player, 3);
    vector<int> d4 = get_directions(player, 4);

    if (d3.empty() || d4.empty())
        return false;

    for (int a : d3)
        for (int b : d4)
            if (a != b) return true;

    return false;
}

bool DiamondBoard::is_draw(Player<char>* /*player*/) {
    return n_moves == 13;  // 13 usable cells in diamond
}

bool DiamondBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// UI ====================================

DiamondUI::DiamondUI()
    : UI<char>("Welcome to Diamond Tic-Tac-Toe!", 2) {}

Player<char>* DiamondUI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type==PlayerType::HUMAN ? "Human" : "Computer")
         << " Player " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* DiamondUI::get_move(Player<char>* player) {
    Board<char>* b = player->get_board_ptr();

    if (player->get_type() == PlayerType::COMPUTER) {
        for (int r = 0; r < 5; r++)
            for (int c = 0; c < 5; c++)
                if (b->get_board_matrix()[r][c] == '.')
                    return new Move<char>(r, c, player->get_symbol());
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int r, c;
    cout << "\nEnter your move (row col): ";
    cin >> r >> c;

    return new Move<char>(r, c, player->get_symbol());
}