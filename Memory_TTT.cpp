/**
 * @file Memory_TTT.cpp
 * @brief Implementation for Memory Tic-Tac-Toe (Board + UI).
 *
 * AI: Minimax full-search (optimal for 3x3).
 */

#include "Memory_TTT.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <climits>

using namespace std;

/* -------------------- Utility input -------------------- */
int Memory_UI::read_int_in_range(int minv, int maxv) {
    int x;
    while (true) {
        if (!(cin >> x)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again: ";
            continue;
        }
        if (x < minv || x > maxv) {
            cout << "Out of range. Enter [" << minv << "-" << maxv << "]: ";
            continue;
        }
        return x;
    }
}

/* -------------------- MemoryBoard -------------------- */

MemoryBoard::MemoryBoard() : Board<char>(3,3), moves_count(0) {
    real_board.assign(3, vector<char>(3, '.'));
}

vector<vector<char>> MemoryBoard::get_board_matrix() const {
    return real_board;
}

bool MemoryBoard::update_board(Move<char>* mv) {
    if (!mv) return false;
    int r = mv->get_x();
    int c = mv->get_y();
    char s = mv->get_symbol();
    if (r < 0 || r >= 3 || c < 0 || c >= 3) return false;
    if (real_board[r][c] != '.') return false;
    real_board[r][c] = s;
    ++moves_count;
    return true;
}

/* Helper: check symbol win on internal board (by char) */
static bool check_win_symbol(const vector<vector<char>>& b, char sym) {
    // rows
    for (int r=0;r<3;++r)
        if (b[r][0]==sym && b[r][1]==sym && b[r][2]==sym) return true;
    // cols
    for (int c=0;c<3;++c)
        if (b[0][c]==sym && b[1][c]==sym && b[2][c]==sym) return true;
    // diags
    if (b[0][0]==sym && b[1][1]==sym && b[2][2]==sym) return true;
    if (b[0][2]==sym && b[1][1]==sym && b[2][0]==sym) return true;
    return false;
}

bool MemoryBoard::is_win(Player<char>* player) {
    if (!player) return false;
    char sym = player->get_symbol();
    return check_win_symbol(real_board, sym);
}

bool MemoryBoard::is_draw(Player<char>* /*player*/) {
    return moves_count >= 9;
}

bool MemoryBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

bool MemoryBoard::is_lose(Player<char>* player) {
    if (!player) return false;
    char sym = player->get_symbol();
    char opp = (sym == 'X') ? 'O' : 'X';
    return check_win_symbol(real_board, opp);
}

/* -------------------- Memory_UI (display + AI) -------------------- */

Memory_UI::Memory_UI() : UI<char>(2) {}

void Memory_UI::display_board_matrix(const vector<vector<char>>& mat) const {
    cout << "    ";
    for (int c = 0; c < 3; ++c) cout << setw(2) << c << " ";
    cout << "\n   +-----------\n";

    for (int r = 0; r < 3; ++r) {
        cout << setw(2) << r << " | ";
        for (int c = 0; c < 3; ++c) {
            if (mat[r][c] == '.') cout << ".  ";
            else cout << "*  "; // hidden mark indicator (ASCII-safe)
        }
        cout << "\n";
    }
    cout << "   +-----------\n";
}

/* -------------------- Minimax AI for 3x3 --------------------
   - Full minimax works well on 3x3 TicTacToe: game tree small.
   - We search recursively, returning score +1 (AI wins), -1 (AI loses), 0 draw.
   - The AI is deterministic and optimal.
------------------------------------------------------------ */

static vector<pair<int,int>> generate_moves(const vector<vector<char>>& b) {
    vector<pair<int,int>> mv;
    for (int r=0;r<3;++r) for (int c=0;c<3;++c) if (b[r][c]=='.') mv.emplace_back(r,c);
    return mv;
}

// minimax: returns score from perspective of 'ai_sym'
static int minimax_rec(vector<vector<char>>& b, char ai_sym, char cur_sym) {
    // terminal?
    if (check_win_symbol(b, ai_sym)) return +1;
    char opp = (ai_sym=='X')?'O':'X';
    if (check_win_symbol(b, opp)) return -1;
    auto moves = generate_moves(b);
    if (moves.empty()) return 0;

    // maximizing if cur_sym == ai_sym
    if (cur_sym == ai_sym) {
        int best = INT_MIN;
        for (auto p : moves) {
            b[p.first][p.second] = cur_sym;
            int sc = minimax_rec(b, ai_sym, (cur_sym=='X')?'O':'X');
            b[p.first][p.second] = '.';
            best = max(best, sc);
            if (best == 1) break; // prune win
        }
        return best;
    } else {
        int best = INT_MAX;
        for (auto p : moves) {
            b[p.first][p.second] = cur_sym;
            int sc = minimax_rec(b, ai_sym, (cur_sym=='X')?'O':'X');
            b[p.first][p.second] = '.';
            best = min(best, sc);
            if (best == -1) break;
        }
        return best;
    }
}

Move<char>* Memory_UI::get_move(Player<char>* player) {
    if (!player) return nullptr;
    Board<char>* bptr = player->get_board_ptr();
    MemoryBoard* mb = dynamic_cast<MemoryBoard*>(bptr);
    if (!mb) {
        // fallback: ask human
        cout << player->get_name() << " (" << player->get_symbol() << ") - enter row and column (0-2): ";
        int r = read_int_in_range(0,2), c = read_int_in_range(0,2);
        return new Move<char>(r,c, player->get_symbol());
    }

    auto mat = mb->get_board_matrix();

    if (player->get_type() == PlayerType::HUMAN) {
        // show hidden board but get move
        display_board_matrix(mat);
        cout << player->get_name() << " (" << player->get_symbol() << ") - enter row and column (0-2): ";
        int r = read_int_in_range(0,2);
        int c = read_int_in_range(0,2);
        while (r<0 || r>2 || c<0 || c>2 || mat[r][c] != '.') {
            cout << "Invalid or occupied. Try again: ";
            r = read_int_in_range(0,2);
            c = read_int_in_range(0,2);
        }
        return new Move<char>(r,c, player->get_symbol());
    } else {
        // COMPUTER: use full minimax (optimal on 3x3)
        char ai_sym = player->get_symbol();
        vector<pair<int,int>> moves = generate_moves(mat);
        if (moves.empty()) return new Move<char>(0,0,ai_sym);

        int bestScore = INT_MIN;
        pair<int,int> bestMove = moves[0];

        for (auto p : moves) {
            mat[p.first][p.second] = ai_sym;
            int sc = minimax_rec(mat, ai_sym, (ai_sym=='X')?'O':'X'); // next turn is opponent
            mat[p.first][p.second] = '.';
            if (sc > bestScore) { bestScore = sc; bestMove = p; }
            if (bestScore == 1) break; // found winning move
        }

        return new Move<char>(bestMove.first, bestMove.second, ai_sym);
    }
}
