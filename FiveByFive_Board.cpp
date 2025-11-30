#include "FiveByFive_Board.h"
#include <cctype>

/**
 * @brief Initializes the 5x5 board with all '.' cells.
 */
FiveByFive_Board::FiveByFive_Board() : Board<char>(5,5) {
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            board[r][c] = blank;

    n_moves = 0;
}

/**
 * @brief Attempts to place a symbol on the board.
 */
bool FiveByFive_Board::update_board(Move<char>* move) {
    if (!move) return false;

    int r = move->get_x();
    int c = move->get_y();
    char sym = move->get_symbol();

    if (isalpha((unsigned char)sym)) 
        sym = toupper(sym);

    if (r < 0 || r >= rows || c < 0 || c >= columns) return false;
    if (board[r][c] != blank) return false;

    board[r][c] = sym;
    n_moves++;
    return true;
}

/**
 * @brief Counts all 3-in-a-row sequences for a symbol.
 */
int FiveByFive_Board::count_three_lines(char symbol) const {
    int cnt = 0;

    // Horizontal
    for (int r = 0; r < 5; ++r)
        for (int c = 0; c <= 2; ++c)
            if (board[r][c] == symbol && board[r][c+1] == symbol && board[r][c+2] == symbol)
                ++cnt;

    // Vertical
    for (int c = 0; c < 5; ++c)
        for (int r = 0; r <= 2; ++r)
            if (board[r][c] == symbol && board[r+1][c] == symbol && board[r+2][c] == symbol)
                ++cnt;

    // Diagonal '\'
    for (int r = 0; r <= 2; ++r)
        for (int c = 0; c <= 2; ++c)
            if (board[r][c] == symbol && board[r+1][c+1] == symbol && board[r+2][c+2] == symbol)
                ++cnt;

    // Diagonal '/'
    for (int r = 0; r <= 2; ++r)
        for (int c = 2; c < 5; ++c)
            if (board[r][c] == symbol && board[r+1][c-1] == symbol && board[r+2][c-2] == symbol)
                ++cnt;

    return cnt;
}

/**
 * @brief True if board has ≤1 empty cell.
 */
bool FiveByFive_Board::full_except_one() const {
    int empty = 0;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            if (board[r][c] == blank) 
                empty++;

    return (empty <= 1);
}

/**
 * @brief Win: more 3-lines than opponent.
 */
bool FiveByFive_Board::is_win(Player<char>* player) {
    if (!player) return false;
    if (!full_except_one()) return false;

    char sym = player->get_symbol();
    char opp = (sym == 'X' ? 'O' : 'X');

    int p = count_three_lines(sym);
    int o = count_three_lines(opp);

    return (p > o);
}

/**
 * @brief Lose: opponent has more 3-lines.
 */
bool FiveByFive_Board::is_lose(Player<char>* player) {
    if (!player) return false;
    if (!full_except_one()) return false;

    char sym = player->get_symbol();
    char opp = (sym == 'X' ? 'O' : 'X');

    int p = count_three_lines(sym);
    int o = count_three_lines(opp);

    return (o > p);
}

/**
 * @brief Draw: both players have equal 3-lines.
 */
bool FiveByFive_Board::is_draw(Player<char>* player) {
    if (!full_except_one()) return false;

    int x = count_three_lines('X');
    int o = count_three_lines('O');

    return (x == o);
}

/**
 * @brief Game over when ≤1 empty cell remains.
 */
bool FiveByFive_Board::game_is_over(Player<char>* /*player*/) {
    return full_except_one();
}
