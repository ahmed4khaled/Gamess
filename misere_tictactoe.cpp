#include "Misere_TICTAKTOE.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>

/* ============================================================
 *   BOARD IMPLEMENTATION
 * ============================================================ */

/**
 * @brief Constructs empty 3×3 board.
 */
MisereBoard::MisereBoard() : Board<char>(3,3) {
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < columns; c++)
            board[r][c] = blank;
    n_moves = 0;
}

/**
 * @brief Attempts to place symbol on board.
 */
bool MisereBoard::update_board(Move<char>* move) {
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
 * @brief Standard 3-in-a-row check.
 */
bool MisereBoard::check_three(char s) const {
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
 * @brief True if board full.
 */
bool MisereBoard::full_board() const {
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (board[r][c] == blank)
                return false;
    return true;
}

/**
 * @brief Player wins if opponent loses.
 */
bool MisereBoard::is_win(Player<char>* player) {
    char me = player->get_symbol();
    char opp = (me == 'X' ? 'O' : 'X');
    return check_three(opp); // opponent formed 3 → I win
}

/**
 * @brief Player loses if they form 3-in-a-row.
 */
bool MisereBoard::is_lose(Player<char>* player) {
    return check_three(player->get_symbol());
}

/**
 * @brief Draw = full board and nobody lost.
 */
bool MisereBoard::is_draw(Player<char>* player) {
    return full_board() &&
        !check_three('X') &&
        !check_three('O');
}

/**
 * @brief Game ends if win, lose, or draw.
 */
bool MisereBoard::game_is_over(Player<char>* p) {
    return is_win(p) || is_lose(p) || is_draw(p);
}

/* ============================================================
 *   MISERE UI IMPLEMENTATION
 * ============================================================ */

MisereUI::MisereUI() : UI<char>(2) {
    std::srand(static_cast<unsigned>(time(nullptr)));
}

/**
 * @brief Safe integer input.
 */
int MisereUI::read_int_in_range(int minv, int maxv) const {
    int x;
    while (true) {
        if (std::cin >> x && x >= minv && x <= maxv) return x;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid! Enter [" << minv << "-" << maxv << "]: ";
    }
}

/**
 * @brief Creates either human or AI player.
 */
Player<char>* MisereUI::create_player(std::string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Displays the 3x3 board.
 */
void MisereUI::display_board_matrix(const std::vector<std::vector<char>>& m) const {
    std::cout << "\n    0   1   2\n";
    std::cout << "  -----------\n";
    for (int r = 0; r < 3; r++) {
        std::cout << r << " |";
        for (int c = 0; c < 3; c++)
            std::cout << " " << m[r][c] << " |";
        std::cout << "\n  -----------\n";
    }
}

/**
 * @brief Gets move from user or AI (random).
 */
Move<char>* MisereUI::get_move(Player<char>* p) {
    Board<char>* b = p->get_board_ptr();
    auto mat = b->get_board_matrix();

    // Human
    if (p->get_type() == PlayerType::HUMAN) {
        std::cout << p->get_name() << " (" << p->get_symbol() << ") enter row 0–2: ";
        int r = read_int_in_range(0,2);

        std::cout << "enter col 0–2: ";
        int c = read_int_in_range(0,2);

        return new Move<char>(r, c, p->get_symbol());
    }

    // AI: pick random empty
    std::vector<std::pair<int,int>> empties;
    for (int r=0;r<3;r++)
        for (int c=0;c<3;c++)
            if (mat[r][c] == '-') empties.emplace_back(r,c);

    if (empties.empty())
        return new Move<char>(0,0,p->get_symbol());

    int k = rand() % empties.size();
    return new Move<char>(empties[k].first, empties[k].second, p->get_symbol());
}
