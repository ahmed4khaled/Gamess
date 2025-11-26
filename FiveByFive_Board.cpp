#include "FiveByFive_Board.h"
#include <cctype>

FiveByFive_Board::FiveByFive_Board() : Board<char>(5,5) {
    // initialize all cells to blank '.'
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            board[r][c] = blank;
    n_moves = 0;
}

bool FiveByFive_Board::update_board(Move<char>* move) {
    if (!move) return false;
    int r = move->get_x();
    int c = move->get_y();
    char sym = move->get_symbol();
    // Normalize symbol to upper-case if it's a letter (consistent with other files)
    if (std::isalpha(static_cast<unsigned char>(sym))) sym = std::toupper(sym);

    if (r < 0 || r >= rows || c < 0 || c >= columns) return false;
    if (board[r][c] != blank) return false;

    board[r][c] = sym;
    n_moves++;
    return true;
}

int FiveByFive_Board::count_three_lines(char symbol) const {
    int cnt = 0;
    // use board via get_board_matrix() (returns copy) or protected member board directly
    // here we use protected member board
    // rows/columns are 5 by constructor
    // Rows
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c <= 2; ++c) {
            if (board[r][c] == symbol && board[r][c+1] == symbol && board[r][c+2] == symbol) ++cnt;
        }
    }
    // Cols
    for (int c = 0; c < 5; ++c) {
        for (int r = 0; r <= 2; ++r) {
            if (board[r][c] == symbol && board[r+1][c] == symbol && board[r+2][c] == symbol) ++cnt;
        }
    }
    // Diag "\"
    for (int r = 0; r <= 2; ++r) {
        for (int c = 0; c <= 2; ++c) {
            if (board[r][c] == symbol && board[r+1][c+1] == symbol && board[r+2][c+2] == symbol) ++cnt;
        }
    }
    // Diag "/"
    for (int r = 0; r <= 2; ++r) {
        for (int c = 2; c < 5; ++c) {
            if (board[r][c] == symbol && board[r+1][c-1] == symbol && board[r+2][c-2] == symbol) ++cnt;
        }
    }
    return cnt;
}

bool FiveByFive_Board::full_except_one() const {
    int empty = 0;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            if (board[r][c] == blank) ++empty;
    return (empty <= 1);
}

bool FiveByFive_Board::is_win(Player<char>* player) {
    if (!player) return false;
    // Only consider win when game finishing condition met
    if (!full_except_one()) return false;

    char sym = player->get_symbol();
    char other = (sym == 'X' ? 'O' : 'X');
    int s1 = count_three_lines(sym);
    int s2 = count_three_lines(other);
    return (s1 > s2);
}

bool FiveByFive_Board::is_lose(Player<char>* player) {
    if (!player) return false;
    if (!full_except_one()) return false;

    char sym = player->get_symbol();
    char other = (sym == 'X' ? 'O' : 'X');
    int s1 = count_three_lines(sym);
    int s2 = count_three_lines(other);
    return (s2 > s1);
}

bool FiveByFive_Board::is_draw(Player<char>* player) {
    // draw when finished and counts equal
    if (!full_except_one()) return false;
    int xcount = count_three_lines('X');
    int ocount = count_three_lines('O');
    return (xcount == ocount);
}

bool FiveByFive_Board::game_is_over(Player<char>* /*player*/) {
    // game ends when only 1 or 0 empty cells remain (as per assignment)
    return full_except_one();
}
