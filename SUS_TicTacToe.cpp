#include "SUS_TicTacToe.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

/**
* @brief SUS_Board constructor initializes a 3x3 board and scores
*/

SUS_Board::SUS_Board() : Board<char>(3, 3), score_S(0), score_U(0) {
    for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) board[r][c] = ' ';
}

bool SUS_Board::in_bounds(int x, int y) const {
    return x >= 0 && x < 3 && y >= 0 && y < 3;
}

char SUS_Board::cell_at(int x, int y) const {
    if (!in_bounds(x, y)) return '\0';
    return board[x][y];
}

/**
* @brief Count number of "SUS" sequences in a 3x3 array board
*/
static int count_SUS_array(const array<array<char, 3>, 3>& b) {
    int cnt = 0;
    for (int r = 0; r < 3; ++r) if (b[r][0] == 'S' && b[r][1] == 'U' && b[r][2] == 'S') ++cnt;
    for (int c = 0; c < 3; ++c) if (b[0][c] == 'S' && b[1][c] == 'U' && b[2][c] == 'S') ++cnt;
    if (b[0][0] == 'S' && b[1][1] == 'U' && b[2][2] == 'S') ++cnt;
    if (b[0][2] == 'S' && b[1][1] == 'U' && b[2][0] == 'S') ++cnt;
    return cnt;
}

/**
* @brief convert vector board to array and count 
*/
static int count_SUS_vec(const vector<vector<char>>& v) {
    array<array<char, 3>, 3> b;
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) b[i][j] = v[i][j];
    return count_SUS_array(b);
}

bool SUS_Board::update_board(Move<char>* move) {
    if (!move) return false;
    int x = move->get_x(), y = move->get_y();
    char sym = move->get_symbol();
    if (!in_bounds(x, y)) return false;
    if (board[x][y] != ' ') return false;
    if (sym != 'S' && sym != 'U') return false;

    int before = count_SUS_vec(board);
    board[x][y] = sym;
    ++n_moves;
    int after = count_SUS_vec(board);
    int delta = after - before;
    if (delta > 0) {
        if (sym == 'S') score_S += delta;
        else score_U += delta;
    }
    return true;
}

bool SUS_Board::game_is_over(Player<char>*) { return n_moves >= 9; }

bool SUS_Board::is_win(Player<char>* player) {
    if (n_moves < 9) return false;
    if (!player) return false;
    char sym = player->get_symbol();
    if (sym == 'S') return score_S > score_U;
    return score_U > score_S;
}

bool SUS_Board::is_lose(Player<char>* player) {
    if (n_moves < 9) return false;
    if (!player) return false;
    char sym = player->get_symbol();
    if (sym == 'S') return score_S < score_U;
    return score_U < score_S;
}

bool SUS_Board::is_draw(Player<char>*) {
    if (n_moves < 9) return false;
    return score_S == score_U;
}

/* ---------------- AI RANDOM ---------------- */
pair<int, int> SUS_Board::random_move() const {
    vector<pair<int, int>> empties;
    for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) if (board[r][c] == ' ') empties.emplace_back(r, c);
    if (empties.empty()) return { -1,-1 };
    int idx = rand() % static_cast<int>(empties.size());
    return empties[idx];
}

/**
* @brief Minimax algorithm with backtracking for optimal move selection
*/

static int minimax_bt(array<array<char, 3>, 3>& b, int moves_made, int sScore, int uScore, char turn) {
    if (moves_made >= 9) return sScore - uScore;

    vector<pair<int, int>> empties;
    for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) if (b[r][c] == ' ') empties.emplace_back(r, c);
    if (empties.empty()) return sScore - uScore;

    if (turn == 'S') {
        int best = numeric_limits<int>::min();
        for (auto [r, c] : empties) {
            int before = count_SUS_array(b);
            b[r][c] = 'S';
            int after = count_SUS_array(b);
            int delta = after - before;
            int val = minimax_bt(b, moves_made + 1, sScore + delta, uScore, 'U');
            b[r][c] = ' ';
            if (val > best) best = val;
        }
        return best;
    }
    else {
        int best = numeric_limits<int>::max();
        for (auto [r, c] : empties) {
            int before = count_SUS_array(b);
            b[r][c] = 'U';
            int after = count_SUS_array(b);
            int delta = after - before;
            int val = minimax_bt(b, moves_made + 1, sScore, uScore + delta, 'S');
            b[r][c] = ' ';
            if (val < best) best = val;
        }
        return best;
    }
}

pair<int, int> SUS_Board::smart_move(char symbol) const {
    array<array<char, 3>, 3> b;
    for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) b[r][c] = board[r][c];

    int bestR = -1, bestC = -1;
    if (symbol == 'S') {
        int bestVal = numeric_limits<int>::min();
        for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) if (b[r][c] == ' ') {
            int before = count_SUS_array(b);
            b[r][c] = 'S';
            int after = count_SUS_array(b);
            int delta = after - before;
            int val = minimax_bt(b, n_moves + 1, score_S + delta, score_U, 'U');
            b[r][c] = ' ';
            if (val > bestVal) { bestVal = val; bestR = r; bestC = c; }
        }
    }
    else {
        int bestVal = numeric_limits<int>::max();
        for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) if (b[r][c] == ' ') {
            int before = count_SUS_array(b);
            b[r][c] = 'U';
            int after = count_SUS_array(b);
            int delta = after - before;
            int val = minimax_bt(b, n_moves + 1, score_S, score_U + delta, 'S');
            b[r][c] = ' ';
            if (val < bestVal) { bestVal = val; bestR = r; bestC = c; }
        }
    }
    return { bestR, bestC };
}

/**
* @brief Create a SUS player, prompting for AI type if COMPUTER
*/

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::COMPUTER) {
        cout << "Choose AI for " << name << ":\n";
        cout << "1. Random\n2. Smart (backtracking)\nChoice: ";
        int ai; cin >> ai;
        SUS_AIType t = (ai == 1 ? SUS_AIType::RANDOM : SUS_AIType::SMART);
        name += (t == SUS_AIType::SMART ? " [AI:Smart]" : " [AI:Random]");
        return new SUS_Player(name, symbol, type, t);
    }
    return new SUS_Player(name, symbol, type, SUS_AIType::RANDOM);
}

Player<char>** SUS_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];

    string n1, n2;
    cout << "Enter Player S name: ";
    cin >> n1;
    cout << "Choose Player S type:\n1. Human\n2. Computer\nChoice: ";
    int t1; cin >> t1;
    players[0] = create_player(n1, 'S', (t1 == 1 ? PlayerType::HUMAN : PlayerType::COMPUTER));

    cout << "Enter Player U name: ";
    cin >> n2;
    cout << "Choose Player U type:\n1. Human\n2. Computer\nChoice: ";
    int t2; cin >> t2;
    players[1] = create_player(n2, 'U', (t2 == 1 ? PlayerType::HUMAN : PlayerType::COMPUTER));

    return players;
}

Move<char>* SUS_UI::get_move(Player<char>* player) {
    if (!player) return nullptr;

    if (player->get_type() == PlayerType::COMPUTER) {
        SUS_Player* sp = dynamic_cast<SUS_Player*>(player);
        SUS_Board* sb = dynamic_cast<SUS_Board*>(player->get_board_ptr());
        if (!sb) {
            // fallback
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) if (player->get_board_ptr()->get_cell(r, c) == ' ')
                return new Move<char>(r, c, player->get_symbol());
            return new Move<char>(0, 0, player->get_symbol());
        }

        if (sp->get_ai_type() == SUS_AIType::RANDOM) {
            auto pos = sb->random_move();
            cout << player->get_name() << " (AI-Random) plays (" << pos.first << "," << pos.second << ")\n";
            return new Move<char>(pos.first, pos.second, player->get_symbol());
        }
        else {
            auto pos = sb->smart_move(player->get_symbol());
            cout << player->get_name() << " (AI-Smart) plays (" << pos.first << "," << pos.second << ")\n";
            return new Move<char>(pos.first, pos.second, player->get_symbol());
        }
    }

    /**
	* @brief input validation for Human player
    */
    int x, y;
    SUS_Board* sb = dynamic_cast<SUS_Board*>(player->get_board_ptr());
    while (true) {
        cout << player->get_name() << " (" << player->get_symbol() << ") enter row and column (0..2): ";
        cin >> x >> y;
        if (!cin) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid input. Try again.\n"; continue; }
        if (x < 0 || x > 2 || y < 0 || y > 2) { cout << "Out of bounds. Use 0..2.\n"; continue; }
        if (sb) {
            if (sb->cell_at(x, y) != ' ') { cout << "Cell occupied. Choose another.\n"; continue; }
        }
        break;
    }
    return new Move<char>(x, y, player->get_symbol());
}
