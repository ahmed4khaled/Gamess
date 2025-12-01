#include "Word_Board.h"
#include <fstream>
#include <algorithm>
#include <cctype>

/**
 * @brief Constructs the board and loads dictionary.
 */
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

/**
 * @brief Loads 3-letter uppercase words into dictionary.
 */
void Word_Board::load_dict(const std::string& path) {
    dict.clear();
    std::ifstream in(path);
    if (!in.is_open()) return;

    std::string w;
    while (in >> w) {
        if (w.size() != 3) continue;
        std::transform(w.begin(), w.end(), w.begin(),
                       [](unsigned char ch){ return std::toupper(ch); });
        dict.insert(w);
    }
}

/**
 * @brief Attempts to place a letter at (r,c) and checks word-formation.
 */
bool Word_Board::update_board(Move<char>* move) {
    if (!move) return false;

    int r = move->get_x();
    int c = move->get_y();
    char ch = move->get_symbol();

    if (std::isalpha(static_cast<unsigned char>(ch)))
        ch = std::toupper(ch);

    if (r < 0 || r >= rows || c < 0 || c >= columns) return false;
    if (board[r][c] != blank) return false;

    // Apply letter
    board[r][c] = ch;
    last_r = r;
    last_c = c;
    ++n_moves;

    // Check if formed a word
    last_move_won = check_three_at(r, c);
    return true;
}

/**
 * @brief Checks all lines passing through (r,c).
 */
bool Word_Board::check_three_at(int r, int c) const {
    std::string s;

    auto valid_word = [&](const std::string& t) {
        std::string up = t;
        std::transform(up.begin(), up.end(), up.begin(),
                       [](unsigned char ch){ return std::toupper(ch); });
        return dict.count(up) > 0;
    };

    // row
    s.clear();
    for (int col = 0; col < 3; ++col) s.push_back(board[r][col]);
    if (s.find(blank) == std::string::npos && valid_word(s)) return true;

    // column
    s.clear();
    for (int row = 0; row < 3; ++row) s.push_back(board[row][c]);
    if (s.find(blank) == std::string::npos && valid_word(s)) return true;

    // main diag
    if (r == c) {
        s.clear();
        for (int i = 0; i < 3; ++i) s.push_back(board[i][i]);
        if (s.find(blank) == std::string::npos && valid_word(s)) return true;
    }

    // anti diag
    if (r + c == 2) {
        s.clear();
        for (int i = 0; i < 3; ++i) s.push_back(board[i][2-i]);
        if (s.find(blank) == std::string::npos && valid_word(s)) return true;
    }

    return false;
}

/**
 * @brief Determines if board is full.
 */
bool Word_Board::full_board() const {
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            if (board[r][c] == blank) return false;
    return true;
}

/**
 * @brief Win if player's last move formed a word.
 */
bool Word_Board::is_win(Player<char>* player) {
    (void)player;
    return last_move_won;
}

/**
 * @brief No losing mechanic in the word game.
 */
bool Word_Board::is_lose(Player<char>* player) {
    (void)player;
    return false;
}

/**
 * @brief Draw if full board and no winner.
 */
bool Word_Board::is_draw(Player<char>* player) {
    (void)player;
    return full_board() && !last_move_won;
}

/**
 * @brief Game ends when someone forms a word or board is full.
 */
bool Word_Board::game_is_over(Player<char>* /*player*/) {
    return last_move_won || full_board();
}
