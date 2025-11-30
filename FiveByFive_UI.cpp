#include "FiveByFive_UI.h"
#include "FiveByFive_Board.h"
#include <iostream>
#include <random>
#include <limits>
#include <ctime>

/**
 * @brief Counts 3-in-a-row occurrences for a symbol in a given matrix.
 */
int FiveByFive_UI::count_three_for_matrix(const std::vector<std::vector<char>>& mat, char symbol) const {
    int R = (int)mat.size();
    int C = (int)mat[0].size();
    int cnt = 0;

    // Horizontal
    for (int r = 0; r < R; ++r)
        for (int c = 0; c <= C - 3; ++c)
            if (mat[r][c] == symbol && mat[r][c+1] == symbol && mat[r][c+2] == symbol) 
                ++cnt;

    // Vertical
    for (int c = 0; c < C; ++c)
        for (int r = 0; r <= R - 3; ++r)
            if (mat[r][c] == symbol && mat[r+1][c] == symbol && mat[r+2][c] == symbol) 
                ++cnt;

    // Diagonal "\"
    for (int r = 0; r <= R - 3; ++r)
        for (int c = 0; c <= C - 3; ++c)
            if (mat[r][c] == symbol && mat[r+1][c+1] == symbol && mat[r+2][c+2] == symbol) 
                ++cnt;

    // Diagonal "/"
    for (int r = 0; r <= R - 3; ++r)
        for (int c = 2; c < C; ++c)
            if (mat[r][c] == symbol && mat[r+1][c-1] == symbol && mat[r+2][c-2] == symbol) 
                ++cnt;

    return cnt;
}

/**
 * @brief Seeds randomness and initializes UI.
 */
FiveByFive_UI::FiveByFive_UI() : UI<char>(2) {
    srand(static_cast<unsigned int>(time(nullptr)));
}

/**
 * @brief Creates a human or computer player.
 */
Player<char>* FiveByFive_UI::create_player(std::string& name, char symbol, PlayerType type) {
    std::cout << "Creating " << (type == PlayerType::HUMAN ? "Human" : "Computer")
              << " Player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Displays the board using parent UI function.
 */
void FiveByFive_UI::display_board_matrix(const std::vector<std::vector<char>>& matrix) const {
    UI<char>::display_board_matrix(matrix);
}

/**
 * @brief Reads an integer safely within a given range.
 */
static int read_int_in_range(int minv, int maxv) {
    int x;
    while (true) {
        if (!(std::cin >> x)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again: ";
            continue;
        }
        if (x < minv || x > maxv) {
            std::cout << "Out of range. Enter [" << minv << "-" << maxv << "]: ";
            continue;
        }
        return x;
    }
}

/**
 * @brief Returns a move from player (human or AI).
 */
Move<char>* FiveByFive_UI::get_move(Player<char>* player) {
    if (!player) return nullptr;

    Board<char>* bptr = player->get_board_ptr();

    // Human Player
    if (player->get_type() == PlayerType::HUMAN) {
        std::cout << player->get_name() << " (" << player->get_symbol()
                  << ") - enter row and column (0–4): ";
        int r = read_int_in_range(0, 4);
        int c = read_int_in_range(0, 4);
        return new Move<char>(r, c, player->get_symbol());
    }

    // Computer Player (AI)
    auto mat = bptr->get_board_matrix();
    std::vector<std::pair<int,int>> empties;

    for (int r = 0; r < 5; ++r)
        for (int c = 0; c < 5; ++c)
            if (mat[r][c] == '.') 
                empties.emplace_back(r, c);

    if (empties.empty())
        return new Move<char>(0, 0, player->get_symbol());

    char sym = player->get_symbol();
    char opp = (sym == 'X' ? 'O' : 'X');

    int bestScore = INT_MIN;
    std::pair<int,int> bestMove = empties[0];

    for (auto pr : empties) {
        int r = pr.first, c = pr.second;

        mat[r][c] = sym;
        int myCount = count_three_for_matrix(mat, sym);

        int worstOpp = INT_MIN;

        for (auto pr2 : empties) {
            if (pr2 == pr) continue;

            int rr = pr2.first, cc = pr2.second;
            if (mat[rr][cc] != '.') continue;

            mat[rr][cc] = opp;
            int oppCount = count_three_for_matrix(mat, opp);
            mat[rr][cc] = '.';

            worstOpp = std::max(worstOpp, oppCount);
        }

        if (worstOpp == INT_MIN)
            worstOpp = 0;

        int distCenter = abs(r - 2) + abs(c - 2);
        int score = (myCount - worstOpp) * 10 - distCenter;

        mat[r][c] = '.';

        if (score > bestScore) {
            bestScore = score;
            bestMove = pr;
        }
    }

    return new Move<char>(bestMove.first, bestMove.second, sym);
}
