#include "Word_Board.h"
#include <fstream>
#include <algorithm>
#include <cctype>

Word_Board::Word_Board(const std::string& dic_path, char blankSymbol)
    : Board<char>(3, 3), blank(blankSymbol)
{
    // initialize board with blank symbol
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            board[r][c] = blank;
    n_moves = 0;
    load_dict(dic_path);
}

void Word_Board::load_dict(const std::string& path) {
    dict.clear();
    std::ifstream in(path);
    if (!in.is_open()) return; // if file missing, dict will be empty -> no word will be matched
    std::string w;
    while (in >> w) {
        if (w.size() != 3) continue;
        // uppercase
        std::transform(w.begin(), w.end(), w.begin(), [](unsigned char ch){ return std::toupper(ch); });
        dict.insert(w);
    }
    in.close();
}

bool Word_Board::update_board(Move<char>* move) {
    if (!move) return false;
    int r = move->get_x();
    int c = move->get_y();
    char ch = move->get_symbol();
    // normalize to uppercase letter
    if (std::isalpha(static_cast<unsigned char>(ch))) ch = std::toupper(ch);

    if (r < 0 || r >= rows || c < 0 || c >= columns) return false;
    if (board[r][c] != blank) return false;

    board[r][c] = ch;
    last_r = r; last_c = c;
    ++n_moves;
    last_move_won = check_three_at(r, c);
    return true;
}

bool Word_Board::check_three_at(int r, int c) const {
    // build strings for row, col, main diag, anti diag if applicable
    std::string s;
    // Row
    s.clear();
    for (int col = 0; col < 3; ++col) s.push_back(board[r][col]);
    if (s.find(blank) == std::string::npos) {
        std::string up = s; std::transform(up.begin(), up.end(), up.begin(), [](unsigned char ch){ return std::toupper(ch); });
        if (dict.count(up)) return true;
    }
    // Col
    s.clear();
    for (int row = 0; row < 3; ++row) s.push_back(board[row][c]);
    if (s.find(blank) == std::string::npos) {
        std::string up = s; std::transform(up.begin(), up.end(), up.begin(), [](unsigned char ch){ return std::toupper(ch); });
        if (dict.count(up)) return true;
    }
    // Main diag (r==c)
    if (r == c) {
        s.clear();
        for (int i = 0; i < 3; ++i) s.push_back(board[i][i]);
        if (s.find(blank) == std::string::npos) {
            std::string up = s; std::transform(up.begin(), up.end(), up.begin(), [](unsigned char ch){ return std::toupper(ch); });
            if (dict.count(up)) return true;
        }
    }
    // Anti diag (r+c==2)
    if (r + c == 2) {
        s.clear();
        for (int i = 0; i < 3; ++i) s.push_back(board[i][2 - i]);
        if (s.find(blank) == std::string::npos) {
            std::string up = s; std::transform(up.begin(), up.end(), up.begin(), [](unsigned char ch){ return std::toupper(ch); });
            if (dict.count(up)) return true;
        }
    }
    return false;
}

bool Word_Board::full_board() const {
    // board full when no blanks
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            if (board[r][c] == blank) return false;
    return true;
}

bool Word_Board::is_win(Player<char>* player) {
    // GameManager calls is_win with the player who just moved.
    // If the last move formed a word, then the mover (current player) wins.
    (void)player; // silence unused param warning
    return last_move_won;
}

bool Word_Board::is_lose(Player<char>* player) {
    // We don't need to assert "lose" here — GameManager will call is_win for the mover.
    // Return false to avoid wrong detections. Alternatively, could return last_move_won && false.
    (void)player;
    return false;
}


bool Word_Board::is_draw(Player<char>* player) {
    // draw if board full and no winner
    (void)player;
    return full_board() && !last_move_won;
}

bool Word_Board::game_is_over(Player<char>* /*player*/) {
    // game ends when someone formed a 3-letter word (last_move_won == true)
    // OR when board is full (no blanks)
    return last_move_won || full_board();
}
