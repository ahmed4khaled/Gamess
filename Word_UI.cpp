#include "Word_UI.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <unordered_set>
#include <fstream>

static std::unordered_set<std::string> WORD_UI_DICT;
static bool WORD_UI_DICT_LOADED = false;

static void load_word_ui_dict(const std::string& path = "dic.txt") {
    if (WORD_UI_DICT_LOADED) return;
    std::ifstream in(path);
    std::string w;
    while (in >> w) {
        if (w.size() != 3) continue;
        std::transform(w.begin(), w.end(), w.begin(), [](unsigned char ch){ return std::toupper(ch); });
        WORD_UI_DICT.insert(w);
    }
    WORD_UI_DICT_LOADED = true;
}
Word_UI::Word_UI() : UI<char>(2) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Player<char>* Word_UI::create_player(std::string& name, char symbol, PlayerType type) {
    // follow framework's expectations for creating players
    return new Player<char>(name, symbol, type);
}

void Word_UI::display_board_matrix(const std::vector<std::vector<char>>& matrix) const {
    std::cout << "\n    0   1   2\n";
    std::cout << "  -------------\n";
    for (int r = 0; r < 3; ++r) {
        std::cout << r << " |";
        for (int c = 0; c < 3; ++c) {
            std::cout << " " << matrix[r][c] << " |";
        }
        std::cout << "\n  -------------\n";
    }
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

int Word_UI::read_int_in_range(int minv, int maxv) const {
    int x;
    while (true) {
        x = safe_read_int();
        if (x >= minv && x <= maxv) return x;
        std::cout << "Out of range, enter [" << minv << "-" << maxv << "]: ";
    }
}

char Word_UI::read_letter() const {
    char ch;
    while (true) {
        std::string s;
        if (!(std::cin >> s)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid. Enter a single letter A-Z: ";
            continue;
        }
        if (s.size() != 1 || !std::isalpha(static_cast<unsigned char>(s[0]))) {
            std::cout << "Invalid. Enter a single letter A-Z: ";
            continue;
        }
        ch = static_cast<char>(std::toupper(s[0]));
        return ch;
    }
}
bool Word_UI::check_word_in_dict(const std::string& s) const {
    if (!WORD_UI_DICT_LOADED) load_word_ui_dict();
    std::string up = s;
    std::transform(up.begin(), up.end(), up.begin(), [](unsigned char ch){ return std::toupper(ch); });
    return WORD_UI_DICT.count(up) > 0;
}

Move<char>* Word_UI::get_move(Player<char>* player) {
    if (!player) return nullptr;
    Board<char>* bptr = player->get_board_ptr();
    if (player->get_type() == PlayerType::HUMAN) {
        std::cout << player->get_name() << " (" << player->get_symbol() << ") - enter row (0-2) and column (0-2) and letter (A-Z)\n";
        std::cout << "row: ";
        int r = read_int_in_range(0,2);
        std::cout << "col: ";
        int c = read_int_in_range(0,2);
        std::cout << "letter: ";
        char ch = read_letter();
        // Return a Move with the letter as symbol (letter placed)
        return new Move<char>(r, c, ch);
    } else {
       // ensure dict loaded
load_word_ui_dict("dic.txt");

auto bptr = player->get_board_ptr();
auto mat = bptr->get_board_matrix();
std::vector<std::pair<int,int>> empties;
for (int r=0;r<3;++r) for (int c=0;c<3;++c) if (mat[r][c]=='-') empties.emplace_back(r,c);
if (empties.empty()) return new Move<char>(0,0,'A');

// helper to uppercase triple
auto up3 = [](char a, char b, char c)->std::string {
    std::string s; s.push_back(std::toupper((unsigned char)a)); s.push_back(std::toupper((unsigned char)b)); s.push_back(std::toupper((unsigned char)c)); return s;
};

// 1) immediate win
for (auto pr : empties) {
    int r = pr.first, c = pr.second;
    for (char L='A'; L<='Z'; ++L) {
        mat[r][c] = L;
        // check row
        if (check_word_in_dict(up3(mat[r][0], mat[r][1], mat[r][2]))) { mat[r][c]='-'; return new Move<char>(r,c,L); }
        if (check_word_in_dict(up3(mat[0][c], mat[1][c], mat[2][c]))) { mat[r][c]='-'; return new Move<char>(r,c,L); }
        if (r==c && check_word_in_dict(up3(mat[0][0], mat[1][1], mat[2][2]))) { mat[r][c]='-'; return new Move<char>(r,c,L); }
        if (r+c==2 && check_word_in_dict(up3(mat[0][2], mat[1][1], mat[2][0]))) { mat[r][c]='-'; return new Move<char>(r,c,L); }
        mat[r][c] = '-';
    }
}

// 2) block opponent immediate win (try to place a letter that prevents opponent making word next)
// simplified: if some empty cell allows opponent a winning move, we try to fill it with any letter that prevents it
for (auto pr : empties) {
    int r = pr.first, c = pr.second;
    bool dangerous = false;
    for (auto q : empties) {
        if (q == pr) continue;
        int rr=q.first, cc=q.second;
        for (char oppL='A'; oppL<='Z'; ++oppL) {
            mat[rr][cc] = oppL;
            if (check_word_in_dict(up3(mat[rr][0], mat[rr][1], mat[rr][2])) ||
                check_word_in_dict(up3(mat[0][cc], mat[1][cc], mat[2][cc])) ||
                (rr==cc && check_word_in_dict(up3(mat[0][0], mat[1][1], mat[2][2]))) ||
                (rr+cc==2 && check_word_in_dict(up3(mat[0][2], mat[1][1], mat[2][0])))) {
                dangerous = true;
            }
            mat[rr][cc] = '-';
            if (dangerous) break;
        }
        if (dangerous) break;
    }
    if (dangerous) {
        // fill this dangerous cell with a random letter (or a letter that doesn't help)
        char pick = 'A' + (std::rand()%26);
        return new Move<char>(r,c,pick);
    }
}

// 3) heuristic: pick cell+letter that maximizes number of compatible dictionary words (simple)
int bestScore = INT_MIN;
std::pair<int,int> bestCell = empties[0];
char bestLetter = 'A';
for (auto pr : empties) {
    int r = pr.first, c = pr.second;
    for (char L='A'; L<='Z'; ++L) {
        mat[r][c] = L;
        int score = 0;
        // count compatible words for row,col,diag(s)
        std::string rowStr = ""; for (int cc=0; cc<3; ++cc) rowStr.push_back(mat[r][cc]);
        for (auto &w : WORD_UI_DICT) {
            bool ok=true;
            for (int i=0;i<3;++i) if (rowStr[i] != '-' && std::toupper((unsigned char)rowStr[i]) != w[i]) { ok=false; break; }
            if (ok) ++score;
        }
        std::string colStr=""; for (int rr=0; rr<3; ++rr) colStr.push_back(mat[rr][c]);
        for (auto &w : WORD_UI_DICT) {
            bool ok=true;
            for (int i=0;i<3;++i) if (colStr[i] != '-' && std::toupper((unsigned char)colStr[i]) != w[i]) { ok=false; break; }
            if (ok) ++score;
        }
        if (r==c) {
            std::string d=""; for (int i=0;i<3;++i) d.push_back(mat[i][i]);
            for (auto &w : WORD_UI_DICT) {
                bool ok=true;
                for (int i=0;i<3;++i) if (d[i] != '-' && std::toupper((unsigned char)d[i]) != w[i]) { ok=false; break; }
                if (ok) ++score;
            }
        }
        if (r+c==2) {
            std::string d=""; for (int i=0;i<3;++i) d.push_back(mat[i][2-i]);
            for (auto &w : WORD_UI_DICT) {
                bool ok=true;
                for (int i=0;i<3;++i) if (d[i] != '-' && std::toupper((unsigned char)d[i]) != w[i]) { ok=false; break; }
                if (ok) ++score;
            }
        }
        mat[r][c] = '-';
        if (score > bestScore) { bestScore = score; bestCell = pr; bestLetter = L; }
    }
}
return new Move<char>(bestCell.first, bestCell.second, bestLetter);

    }
}
