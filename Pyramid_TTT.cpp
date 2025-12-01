
#include "Pyramid_TTT.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

// ---------------- PyramidBoard ----------------

PyramidBoard::PyramidBoard() : Board<char>(3, 5) {
    // Initialize all to '#'
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            board[r][c] = '#';

    // valid cells: row0 col2, row1 cols1..3, row2 cols0..4
    board[0][2] = '.';
    board[1][1] = board[1][2] = board[1][3] = '.';
    for (int c = 0; c < 5; ++c) board[2][c] = '.';

    n_moves = 0;
}

std::vector<std::vector<char>> PyramidBoard::get_board_matrix() const {
    return board;
}

bool PyramidBoard::inside_bounds(int r, int c) const {
    return (r >= 0 && r < rows && c >= 0 && c < columns);
}

bool PyramidBoard::is_valid_cell(int r, int c) const {
    if (!inside_bounds(r,c)) return false;
    return board[r][c] != '#';
}

/**
 * update_board: MUST accept Move<char>* as in framework
 * Use Move::get_x(), get_y(), get_symbol()
 */
bool PyramidBoard::update_board(Move<char>* mv) {
    if (!mv) return false;
    int r = mv->get_x();
    int c = mv->get_y();
    char sym = mv->get_symbol();

    if (!inside_bounds(r,c)) return false;
    if (!is_valid_cell(r,c)) return false;
    if (board[r][c] != '.') return false;

    board[r][c] = sym;
    ++n_moves;
    return true;
}

/**
 * is_win: must match signature is_win(Player<char>*)
 * Use player->get_symbol() to obtain symbol to check.
 */
bool PyramidBoard::is_win(Player<char>* player) {
    if (!player) return false;
    char symbol = player->get_symbol();

    // directions: right, down, diag down-right, diag down-left
    const int dirs[4][2] = { {0,1}, {1,0}, {1,1}, {1,-1} };

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (!is_valid_cell(r,c)) continue;
            if (board[r][c] != symbol) continue;
            for (int d = 0; d < 4; ++d) {
                int dr = dirs[d][0], dc = dirs[d][1];
                int r1 = r + dr, c1 = c + dc;
                int r2 = r + 2*dr, c2 = c + 2*dc;
                if (!inside_bounds(r1,c1) || !inside_bounds(r2,c2)) continue;
                if (!is_valid_cell(r1,c1) || !is_valid_cell(r2,c2)) continue;
                if (board[r1][c1] == symbol && board[r2][c2] == symbol) return true;
            }
        }
    }
    return false;
}

bool PyramidBoard::is_draw(Player<char>* /*player*/) {
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            if (is_valid_cell(r,c) && board[r][c] == '.') return false;
    return true;
}

bool PyramidBoard::game_is_over(Player<char>* player) {
    // Framework's GameManager will call is_win/is_draw on players, but implementing
    // as convenient:
    return is_win(player) || is_draw(player);
}

// ---------------- Pyramid_UI ----------------

Pyramid_UI::Pyramid_UI() : UI<char>(2) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

int Pyramid_UI::read_int_in_range(int minv, int maxv) {
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

Move<char>* Pyramid_UI::ask_human(Player<char>* player) {
    cout << "Enter row (0-2) and col (0-4): ";
    int r = read_int_in_range(0,2);
    int c = read_int_in_range(0,4);
    return new Move<char>(r, c, player->get_symbol());
}

void Pyramid_UI::display_board_matrix(const std::vector<std::vector<char>>& mat) const {
    cout << "   Columns: 0 1 2 3 4\n";
    cout << "   ------------------\n";
    for (int r = 0; r < (int)mat.size(); ++r) {
        cout << r << " | ";
        for (int c = 0; c < (int)mat[r].size(); ++c) {
            if (mat[r][c] == '#') cout << "  ";
            else cout << mat[r][c] << ' ';
        }
        cout << '\n';
    }
    cout << "   ------------------\n";
}

Move<char>* Pyramid_UI::get_move(Player<char>* player) {
    if (!player) return nullptr;
    Board<char>* bptr = player->get_board_ptr();
    PyramidBoard* pb = dynamic_cast<PyramidBoard*>(bptr);
    if (!pb) return ask_human(player);

    auto mat = pb->get_board_matrix();
    if (player->get_type() == PlayerType::HUMAN) {
        display_board_matrix(mat);
        cout << player->get_name() << " (" << player->get_symbol() << ") - enter row and column (0..2,0..4): ";
        int r = read_int_in_range(0,2);
        int c = read_int_in_range(0,4);
        while (!pb->is_valid_cell(r,c) || mat[r][c] != '.') {
            cout << "Invalid cell or occupied. Try again: ";
            r = read_int_in_range(0,2);
            c = read_int_in_range(0,4);
        }
        return new Move<char>(r, c, player->get_symbol());
    } else {
        // simple COMPUTER policy: win if possible, block else center preference else first empty
        char sym = player->get_symbol();
        char opp = (sym == 'X' ? 'O' : 'X');

        // 1: try winning move
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 5; ++c)
                if (pb->is_valid_cell(r,c) && mat[r][c] == '.') {
                    mat[r][c] = sym;
                    if (pb->is_win(player)) { mat[r][c] = '.'; return new Move<char>(r,c,sym); }
                    mat[r][c] = '.';
                }

        // 2: block opponent
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 5; ++c)
                if (pb->is_valid_cell(r,c) && mat[r][c] == '.') {
                    mat[r][c] = opp;
                    // create a temporary Player object to call is_win with opponent symbol
                    Player<char> tempOpp("tmp", opp, PlayerType::COMPUTER);
                    tempOpp.set_board_ptr(pb);
                    if (pb->is_win(&tempOpp)) { mat[r][c] = '.'; return new Move<char>(r,c,sym); }
                    mat[r][c] = '.';
                }

        // 3: preferred centers
        std::vector<std::pair<int,int>> pref = {{1,2},{2,2},{2,1},{2,3},{0,2}};
        for (auto p : pref) if (pb->is_valid_cell(p.first,p.second) && mat[p.first][p.second]=='.')
            return new Move<char>(p.first,p.second,sym);

        // 4: fallback first empty
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 5; ++c)
                if (pb->is_valid_cell(r,c) && mat[r][c]=='.') return new Move<char>(r,c,sym);

        // should not happen
        return new Move<char>(0,2,sym);
    }
}

