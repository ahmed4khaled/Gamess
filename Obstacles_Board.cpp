#include "Obstacles_Board.h"
#include <chrono>
#include <algorithm>

Obstacles_Board::Obstacles_Board(char blankSymbol, char obstacleSymbol)
    : Board<char>(6, 6), obstacle(obstacleSymbol), blank(blankSymbol)
{
    rng.seed(static_cast<unsigned long>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            board[r][c] = blank;

    n_moves = 0;
    last_r = last_c = -1;
    last_symbol = 0;
    last_move_created_win = false;
}

std::vector<std::vector<char>> Obstacles_Board::get_board_matrix() const {
    return board;
}

bool Obstacles_Board::is_cell_free(int r, int c) const {
    if (r < 0 || r >= rows || c < 0 || c >= columns) return false;
    char ch = board[r][c];
    if (ch == obstacle) return false;
    // treat blank as free
    return ch == blank;
}

bool Obstacles_Board::update_board(Move<char>* move) {
    if (!move) return false;
    int r = move->get_x();
    int c = move->get_y();
    char sym = move->get_symbol();

    if (r < 0 || r >= rows || c < 0 || c >= columns) return false;
    if (!is_cell_free(r, c)) return false;

    // place the symbol
    board[r][c] = sym;
    ++n_moves;

    // record last move info and check for win BEFORE adding obstacles
    last_r = r; last_c = c; last_symbol = sym;
    last_move_created_win = (count_four_in_row(sym) > 0);

    // Only add obstacles if the game has NOT just been won
    if (!last_move_created_win && n_moves % 2 == 0) {
        add_random_obstacles(2);
    }

    return true;
}

void Obstacles_Board::add_random_obstacles(int count) {
    for (int i = 0; i < count; ++i) {
        if (!place_single_random_obstacle()) break;
    }
}

bool Obstacles_Board::place_single_random_obstacle() {
    std::vector<std::pair<int,int>> empties;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            if (board[r][c] == blank)
                empties.emplace_back(r,c);

    if (empties.empty()) return false;
    std::uniform_int_distribution<int> dist(0, (int)empties.size() - 1);
    auto pr = empties[dist(rng)];
    board[pr.first][pr.second] = obstacle;
    return true;
}

int Obstacles_Board::count_four_in_row(char symbol) const {
    int cnt = 0;
    int R = rows, C = columns;
    // rows
    for (int r = 0; r < R; ++r)
        for (int c = 0; c <= C - 4; ++c) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) if (board[r][c + k] != symbol) { ok = false; break; }
            if (ok) ++cnt;
        }
    // cols
    for (int c = 0; c < C; ++c)
        for (int r = 0; r <= R - 4; ++r) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) if (board[r + k][c] != symbol) { ok = false; break; }
            if (ok) ++cnt;
        }
    // diag "\"
    for (int r = 0; r <= R - 4; ++r)
        for (int c = 0; c <= C - 4; ++c) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) if (board[r + k][c + k] != symbol) { ok = false; break; }
            if (ok) ++cnt;
        }
    // diag "/"
    for (int r = 0; r <= R - 4; ++r)
        for (int c = 3; c < C; ++c) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) if (board[r + k][c - k] != symbol) { ok = false; break; }
            if (ok) ++cnt;
        }
    return cnt;
}

bool Obstacles_Board::board_full_except_obstacles() const {
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            if (board[r][c] == blank) return false;
    return true;
}

bool Obstacles_Board::is_win(Player<char>* player) {
    if (!player) return false;
    // last_move_created_win indicates the last mover created a 4-in-row
    return last_move_created_win && (player->get_symbol() == last_symbol);
}

bool Obstacles_Board::is_lose(Player<char>* player) {
    if (!player) return false;
    // player loses if last move by opponent created the win
    return last_move_created_win && (player->get_symbol() != last_symbol);
}

bool Obstacles_Board::is_draw(Player<char>* /*player*/) {
    // draw if board full of non-blank or no one has 4-in-row
    return board_full_except_obstacles() && !(count_four_in_row('X') > 0 || count_four_in_row('O') > 0);
}

bool Obstacles_Board::game_is_over(Player<char>* /*player*/) {
    return last_move_created_win || board_full_except_obstacles();
}
