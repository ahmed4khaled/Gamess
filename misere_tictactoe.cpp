#include <iostream>
#include <cctype>
#include "Misere_TICTAKTOE.h"

using namespace std;

// ---------------- MisereBoard implementation ----------------

MisereBoard::MisereBoard() : Board<char>(3, 3) {
    for (auto &r : board)
        for (auto &c : r)
            c = blank_symbol;
}

bool MisereBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { 
            n_moves--;
            board[x][y] = blank_symbol;
        } else {
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

bool MisereBoard::has_three_in_row(char s) const {
    for (int i = 0; i < rows; ++i)
        if (board[i][0] == s && board[i][1] == s && board[i][2] == s)
            return true;

    for (int j = 0; j < columns; ++j)
        if (board[0][j] == s && board[1][j] == s && board[2][j] == s)
            return true;

    if (board[0][0] == s && board[1][1] == s && board[2][2] == s) return true;
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;

    return false;
}

bool MisereBoard::is_win(Player<char>* /*player*/) {
    return false;
}

bool MisereBoard::is_lose(Player<char>* player) {
    char s = toupper(player->get_symbol());
    return has_three_in_row(s);
}

bool MisereBoard::is_draw(Player<char>* /*player*/) {
    bool x_three = has_three_in_row('X');
    bool o_three = has_three_in_row('O');

    return (n_moves == rows * columns) && !x_three && !o_three;
}

bool MisereBoard::game_is_over(Player<char>* player) {
    return is_lose(player) || is_draw(player);
}

// ---------------- MisereUI implementation ----------------

MisereUI::MisereUI()
    : UI<char>("Welcome to Misère Tic-Tac-Toe (avoid making 3-in-a-row!)", 3) {}

Player<char>* MisereUI::create_player(std::string& name, char symbol, PlayerType type) {
    cout << "Creating " 
         << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* MisereUI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nEnter your move (x y): ";
        cin >> x >> y;
    } else {
        x = rand() % 3;
        y = rand() % 3;
    }

    return new Move<char>(x, y, player->get_symbol());
}
