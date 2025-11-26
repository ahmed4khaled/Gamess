#include "Obstacles_UI.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <algorithm>

Obstacles_UI::Obstacles_UI() : UI<char>(2) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Player<char>* Obstacles_UI::create_player(std::string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

void Obstacles_UI::display_board_matrix(const std::vector<std::vector<char>>& matrix) const {
    int R = (int)matrix.size();
    int C = (int)matrix[0].size();
    std::cout << "\n   ";
    for (int c = 0; c < C; ++c) std::cout << c << ' ';
    std::cout << "\n  ";
    for (int c = 0; c < C; ++c) std::cout << "--";
    std::cout << "\n";
    for (int r = 0; r < R; ++r) {
        std::cout << r << "| ";
        for (int c = 0; c < C; ++c) {
            char ch = matrix[r][c];
            std::cout << ch << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

static int safe_read_int() {
    int x;
    while (true) {
        if (std::cin >> x) return x;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input, enter a number: ";
    }
}

int Obstacles_UI::read_int_in_range(int minv, int maxv) const {
    int x;
    while (true) {
        x = safe_read_int();
        if (x >= minv && x <= maxv) return x;
        std::cout << "Out of range, enter [" << minv << "-" << maxv << "]: ";
    }
}

// Count 4-in-row windows for given matrix and symbol
int Obstacles_UI::count_four_for_matrix(const std::vector<std::vector<char>>& mat, char symbol) const {
    int R = (int)mat.size();
    int C = (int)mat[0].size();
    int cnt = 0;
    // rows
    for (int r = 0; r < R; ++r)
        for (int c = 0; c <= C - 4; ++c) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) if (mat[r][c + k] != symbol) { ok = false; break; }
            if (ok) ++cnt;
        }
    // cols
    for (int c = 0; c < C; ++c)
        for (int r = 0; r <= R - 4; ++r) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) if (mat[r + k][c] != symbol) { ok = false; break; }
            if (ok) ++cnt;
        }
    // diag "\"
    for (int r = 0; r <= R - 4; ++r)
        for (int c = 0; c <= C - 4; ++c) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) if (mat[r + k][c + k] != symbol) { ok = false; break; }
            if (ok) ++cnt;
        }
    // diag "/"
    for (int r = 0; r <= R - 4; ++r)
        for (int c = 3; c < C; ++c) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) if (mat[r + k][c - k] != symbol) { ok = false; break; }
            if (ok) ++cnt;
        }
    return cnt;
}

// Evaluate a single candidate move (r,c) for AI:
// returns an integer score (higher better).
// Strategy:
//  - if move immediately creates a 4-in-row for AI -> huge positive score
//  - simulate opponent best immediate reply and subtract opponent's gained 4-in-row
//  - add small preference toward center and toward moves that block opponent threats
int Obstacles_UI::evaluate_move_for_ai(std::vector<std::vector<char>>& mat, int r, int c, char mySym, char oppSym) const {
    int R = (int)mat.size();
    int C = (int)mat[0].size();

    // safety
    if (r < 0 || r >= R || c < 0 || c >= C) return INT_MIN;
    if (mat[r][c] == '#' || mat[r][c] == mySym || mat[r][c] == oppSym) return INT_MIN;

    // simulate placing my symbol
    mat[r][c] = mySym;
    int myCount = count_four_for_matrix(mat, mySym);
    // immediate win
    if (myCount > 0) {
        mat[r][c] = '.'; // revert
        return 1000000; // very large score to prefer immediate wins
    }

    // simulate opponent best immediate reply (maximize opponent's 4-in-row)
    int worstOpp = 0;
    // collect empties
    std::vector<std::pair<int,int>> empties;
    for (int rr = 0; rr < R; ++rr)
        for (int cc = 0; cc < C; ++cc)
            if (mat[rr][cc] != '#' && mat[rr][cc] != mySym && mat[rr][cc] != oppSym)
                empties.emplace_back(rr, cc);

    for (auto &pr : empties) {
        int orr = pr.first, occ = pr.second;
        mat[orr][occ] = oppSym;
        int oppCount = count_four_for_matrix(mat, oppSym);
        worstOpp = std::max(worstOpp, oppCount);
        mat[orr][occ] = '.'; // revert
        if (worstOpp > 0) break; // no need to continue if opponent can win immediately
    }

    // heuristic components:
    // prefer center-ish cells
    int centerR = R/2, centerC = C/2;
    int distCenter = abs(r - centerR) + abs(c - centerC);
    int centerScore = ( (R + C) - distCenter ); // bigger for center

    // blocking potential: check how many opponent-3-in-row patterns would be broken by placing here
    int blockScore = 0;
    // For simplicity: count opponent-possible-3 sequences that include (r,c) when it's blank
    // i.e. for every window of 4 that contains (r,c), if by placing mySym we reduce opp's potential
    // We'll approximate by counting opponent partial windows (3 of opp + 1 blank) that include (r,c)
    auto is_valid_cell = [&](int rr,int cc)->bool { return rr>=0 && rr<R && cc>=0 && cc<C; };

    // check rows windows of length 4 that include (r,c)
    for (int start = c - 3; start <= c; ++start) {
        int s = start;
        int e = start + 3;
        if (s < 0 || e >= C) continue;
        int oppCount = 0, blankCount = 0;
        for (int k = s; k <= e; ++k) {
            if (mat[r][k] == oppSym) ++oppCount;
            else if (mat[r][k] == '.') ++blankCount;
            else if (mat[r][k] == '#') { oppCount = -100; break; } // obstacle breaks
        }
        if (oppCount == 3 && blankCount == 1) ++blockScore;
    }
    // columns
    for (int start = r - 3; start <= r; ++start) {
        int s = start;
        int e = start + 3;
        if (s < 0 || e >= R) continue;
        int oppCount = 0, blankCount = 0;
        for (int k = s; k <= e; ++k) {
            if (mat[k][c] == oppSym) ++oppCount;
            else if (mat[k][c] == '.') ++blankCount;
            else if (mat[k][c] == '#') { oppCount = -100; break; }
        }
        if (oppCount == 3 && blankCount == 1) ++blockScore;
    }
    // diag "\"
    for (int dr = -3; dr <= 0; ++dr) {
        int sr = r + dr, sc = c + dr;
        int er = sr + 3, ec = sc + 3;
        if (!is_valid_cell(sr,sc) || !is_valid_cell(er,ec)) continue;
        int oppCount = 0, blankCount = 0;
        for (int k = 0; k < 4; ++k) {
            char ch = mat[sr + k][sc + k];
            if (ch == oppSym) ++oppCount;
            else if (ch == '.') ++blankCount;
            else if (ch == '#') { oppCount = -100; break; }
        }
        if (oppCount == 3 && blankCount == 1) ++blockScore;
    }
    // diag "/"
    for (int dr = -3; dr <= 0; ++dr) {
        int sr = r + dr, sc = c - dr;
        int er = sr + 3, ec = sc - 3;
        if (!is_valid_cell(sr,sc) || !is_valid_cell(er,ec)) continue;
        int oppCount = 0, blankCount = 0;
        for (int k = 0; k < 4; ++k) {
            char ch = mat[sr + k][sc - k];
            if (ch == oppSym) ++oppCount;
            else if (ch == '.') ++blankCount;
            else if (ch == '#') { oppCount = -100; break; }
        }
        if (oppCount == 3 && blankCount == 1) ++blockScore;
    }

    // final score composition
    int score = 0;
    // prefer moves that leave opponent with fewer immediate wins
    score += (0 - worstOpp) * 500;
    // small bonus for center proximity
    score += centerScore * 5;
    // bonus for blocking threats
    score += blockScore * 300;

    // small tiebreaker random
    score += (std::rand() % 10);

    // revert the simulation
    mat[r][c] = '.';
    return score;
}

Move<char>* Obstacles_UI::get_move(Player<char>* player) {
    if (!player) return nullptr;
    Board<char>* bptr = player->get_board_ptr();
    auto mat = bptr->get_board_matrix();
    int R = (int)mat.size();
    int C = (int)mat[0].size();

    if (player->get_type() == PlayerType::HUMAN) {
        while (true) {
            std::cout << player->get_name() << " (" << player->get_symbol() << ") - enter row and column (0-" << R-1 << ")\n";
            std::cout << "row: ";
            int r = read_int_in_range(0, R-1);
            std::cout << "col: ";
            int c = read_int_in_range(0, C-1);

            // refresh matrix in case it changed
            mat = bptr->get_board_matrix();
            if (!bptr) return nullptr; // safety

            char cell = mat[r][c];
            if (cell == '#' || cell == 'X' || cell == 'O') {
                std::cout << "Invalid move! Cell is blocked or already taken. Try again.\n";
                continue;
            }
            return new Move<char>(r, c, player->get_symbol());
        }
    } else {
        // COMPUTER: use heuristic evaluation to choose best move
        char mySym = player->get_symbol();
        char oppSym = (mySym == 'X') ? 'O' : 'X';

        // build list of empties ('.' assumed blank)
        std::vector<std::pair<int,int>> empties;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                if (mat[r][c] != '#' && mat[r][c] != 'X' && mat[r][c] != 'O')
                    empties.emplace_back(r,c);

        if (empties.empty()) return new Move<char>(0,0, mySym);

        // 1) immediate winning move?
        for (auto &pr : empties) {
            int r = pr.first, c = pr.second;
            // simulate on a copy
            auto copyMat = mat;
            copyMat[r][c] = mySym;
            if (count_four_for_matrix(copyMat, mySym) > 0) {
                return new Move<char>(r, c, mySym);
            }
        }

        // 2) block opponent immediate wins (if opponent can win next move)
        for (auto &pr : empties) {
            int r = pr.first, c = pr.second;
            // simulate placing here and see if opponent has immediate win anywhere (we prefer to block)
            auto copyMat = mat;
            copyMat[r][c] = mySym;
            bool oppCanWin = false;
            // scan opponent replies
            for (int rr = 0; rr < R && !oppCanWin; ++rr)
                for (int cc = 0; cc < C && !oppCanWin; ++cc)
                    if (copyMat[rr][cc] != '#' && copyMat[rr][cc] != mySym && copyMat[rr][cc] != oppSym) {
                        copyMat[rr][cc] = oppSym;
                        if (count_four_for_matrix(copyMat, oppSym) > 0) oppCanWin = true;
                        copyMat[rr][cc] = '.';
                    }
            if (!oppCanWin) {
                // prefer moves that avoid opponent immediate win
                // evaluate more precisely by evaluate_move_for_ai
                int sc = evaluate_move_for_ai(copyMat, r, c, mySym, oppSym);
                // if score is reasonable pick it (we'll compute best later; here we just mark safe moves)
                (void)sc;
            }
        }

        // 3) full evaluation (depth-1 lookahead)
        int bestScore = INT_MIN;
        std::pair<int,int> bestMove = empties[0];
        for (auto &pr : empties) {
            int r = pr.first, c = pr.second;
            int sc = evaluate_move_for_ai(mat, r, c, mySym, oppSym);
            if (sc > bestScore) {
                bestScore = sc;
                bestMove = pr;
            }
        }

        return new Move<char>(bestMove.first, bestMove.second, mySym);
    }
}
