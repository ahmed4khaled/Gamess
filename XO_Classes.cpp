#include "XO_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

/* ============================================================
 *  X_O_Board IMPLEMENTATION
 * ============================================================ */

/**
 * @brief Constructs empty 3×3 Tic-Tac-Toe board.
 */
X_O_Board::X_O_Board() : Board<char>(3, 3) {
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < columns; c++)
            board[r][c] = blank;
    n_moves = 0;
}

/**
 * @brief Places symbol at (r,c) if valid.
 */
bool X_O_Board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char s = move->get_symbol();

    if (r < 0 || r >= rows || c < 0 || c >= columns)
        return false;

    if (board[r][c] != blank)
        return false;

    board[r][c] = s;
    n_moves++;
    return true;
}

/**
 * @brief Checks all 3-in-a-row possibilities.
 */
bool X_O_Board::check_win(char s) const {
    // rows
    for (int r = 0; r < 3; r++)
        if (board[r][0] == s && board[r][1] == s && board[r][2] == s)
            return true;

    // columns
    for (int c = 0; c < 3; c++)
        if (board[0][c] == s && board[1][c] == s && board[2][c] == s)
            return true;

    // diag ↘
    if (board[0][0] == s && board[1][1] == s && board[2][2] == s)
        return true;

    // diag ↙
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s)
        return true;

    return false;
}

/**
 * @brief Player wins if they have 3-in-a-row.
 */
bool X_O_Board::is_win(Player<char>* player) {
    return check_win(player->get_symbol());
}

/**
 * @brief Lose if opponent has 3-in-a-row.
 */
bool X_O_Board::is_lose(Player<char>* player) {
    char opp = (player->get_symbol() == 'X' ? 'O' : 'X');
    return check_win(opp);
}

/**
 * @brief True if board is full.
 */
bool X_O_Board::full_board() const {
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (board[r][c] == blank)
                return false;
    return true;
}

/**
 * @brief Draw if full and no winner.
 */
bool X_O_Board::is_draw(Player<char>* player) {
    return full_board() &&
           !check_win('X') &&
           !check_win('O');
}

/**
 * @brief Win or draw ends game.
 */
bool X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

/* ============================================================
 *  XO_UI IMPLEMENTATION
 * ============================================================ */

/**
 * @brief Initializes UI, seeds RNG.
 */
XO_UI::XO_UI() : UI<char>(2) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

/**
 * @brief Reads integer safely in range.
 */
int XO_UI::read_int_in_range(int minv, int maxv) const {
    int x;
    while (true) {
        if (std::cin >> x && x >= minv && x <= maxv)
            return x;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid! Enter [" << minv << "-" << maxv << "]: ";
    }
}

/**
 * @brief Creates human or AI player.
 */
Player<char>* XO_UI::create_player(std::string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Displays board neatly.
 */
void XO_UI::display_board_matrix(const std::vector<std::vector<char>>& matrix) const {

    std::cout << "\n    0   1   2\n";
    std::cout << "  -----------\n";

    for (int r = 0; r < 3; r++) {
        std::cout << r << " |";
        for (int c = 0; c < 3; c++)
            std::cout << " " << matrix[r][c] << " |";
        std::cout << "\n  -----------\n";
    }
}

/**
 * @brief Gets move from human or computer player.
 *
 * AI strategy:
 * - Picks a random empty cell.
 */
Move<char>* XO_UI::get_move(Player<char>* player) {
    Board<char>* bptr = player->get_board_ptr();
    const auto mat = bptr->get_board_matrix();

    // ---------- HUMAN ----------
    if (player->get_type() == PlayerType::HUMAN) {
        std::cout << player->get_name() << " (" << player->get_symbol()
                  << ") enter row (0–2): ";
        int r = read_int_in_range(0, 2);

        std::cout << "enter col (0–2): ";
        int c = read_int_in_range(0, 2);

        return new Move<char>(r, c, player->get_symbol());
    }

    // ---------- COMPUTER ----------
    std::vector<std::pair<int,int>> empties;
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (mat[r][c] == '-')
                empties.emplace_back(r, c);

    if (empties.empty())
        return new Move<char>(0, 0, player->get_symbol());

    int k = std::rand() % empties.size();
    return new Move<char>(empties[k].first, empties[k].second, player->get_symbol());
}
