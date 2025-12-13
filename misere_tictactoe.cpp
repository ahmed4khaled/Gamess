#include "Misere_TICTAKTOE.h"
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

/* ============================================================
 *                     BOARD IMPLEMENTATION
 * ============================================================ */

MisereBoard::MisereBoard() : Board<char>(3,3) {
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            board[r][c] = blank;
    n_moves = 0;
}

bool MisereBoard::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char s = move->get_symbol();

    if (r < 0 || r >= 3 || c < 0 || c >= 3) return false;
    if (board[r][c] != blank) return false;

    board[r][c] = s;
    n_moves++;
    return true;
}

void MisereBoard::make_move(int r, int c, char s) {
    board[r][c] = s;
    n_moves++;
}

void MisereBoard::undo_move(int r, int c) {
    board[r][c] = blank;
    n_moves--;
}

vector<pair<int,int>> MisereBoard::get_available_moves() const {
    vector<pair<int,int>> moves;
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (board[r][c] == blank)
                moves.push_back({r, c});
    return moves;
}

bool MisereBoard::check_three(char s) const {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == s && board[i][1] == s && board[i][2] == s) return true;
        if (board[0][i] == s && board[1][i] == s && board[2][i] == s) return true;
    }
    if (board[0][0] == s && board[1][1] == s && board[2][2] == s) return true;
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;
    return false;
}

bool MisereBoard::full_board() const {
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (board[r][c] == blank)
                return false;
    return true;
}

bool MisereBoard::is_win(Player<char>* player) {
    char me = player->get_symbol();
    char opp = (me == 'X' ? 'O' : 'X');
    return check_three(opp);
}

bool MisereBoard::is_lose(Player<char>* player) {
    return check_three(player->get_symbol());
}

bool MisereBoard::is_draw(Player<char>* /*player*/) {
    return full_board() && !check_three('X') && !check_three('O');
}

bool MisereBoard::game_is_over(Player<char>* p) {
    return is_win(p) || is_lose(p) || is_draw(p);
}

/* ============================================================
 *                     MINIMAX AI
 * ============================================================ */

MisereAIPlayer::MisereAIPlayer(string name, char symbol)
    : Player<char>(name, symbol, PlayerType::COMPUTER) {}

int MisereAIPlayer::minimax(MisereBoard* board, bool isMaximizing) {
    char ai = symbol;
    char human = (ai == 'X' ? 'O' : 'X');

    if (board->check_three(ai)) return -10;     // AI loses
    if (board->check_three(human)) return +10;  // Human loses
    if (board->full_board()) return 0;

    auto moves = board->get_available_moves();

    if (isMaximizing) {
        int best = -1000;
        for (auto& m : moves) {
            board->make_move(m.first, m.second, ai);
            int score = minimax(board, false);
            board->undo_move(m.first, m.second);
            best = max(best, score);
        }
        return best;
    } else {
        int worst = 1000;
        for (auto& m : moves) {
            board->make_move(m.first, m.second, human);
            int score = minimax(board, true);
            board->undo_move(m.first, m.second);
            worst = min(worst, score);
        }
        return worst;
    }
}

Move<char>* MisereAIPlayer::get_best_move() {
    auto* board = dynamic_cast<MisereBoard*>(boardPtr);
    if (!board) return nullptr;

    int bestScore = -1000;
    pair<int,int> bestMove = {0,0};

    for (auto& m : board->get_available_moves()) {
        board->make_move(m.first, m.second, symbol);
        int score = minimax(board, false);
        board->undo_move(m.first, m.second);

        if (score > bestScore) {
            bestScore = score;
            bestMove = m;
        }
    }

    return new Move<char>(bestMove.first, bestMove.second, symbol);
}

/* ============================================================
 *                     UI IMPLEMENTATION
 * ============================================================ */

MisereUI::MisereUI() : UI<char>(2) {}

int MisereUI::read_int_in_range(int minv, int maxv) const {
    int x;
    while (true) {
        if (cin >> x && x >= minv && x <= maxv) return x;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid! Enter [" << minv << "-" << maxv << "]: ";
    }
}

Player<char>* MisereUI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::COMPUTER)
        return new MisereAIPlayer(name, symbol);
    return new Player<char>(name, symbol, type);
}

void MisereUI::display_rules() const {
    cout << "\nMISERE TIC-TAC-TOE RULES\n";
    cout << "- Making 3-in-a-row means you LOSE.\n";
    cout << "- Force your opponent to complete a line.\n\n";
}

void MisereUI::display_board_matrix(const vector<vector<char>>& m) const {
    cout << "\n  0 1 2\n";
    for (int r = 0; r < 3; r++) {
        cout << r << " ";
        for (int c = 0; c < 3; c++)
            cout << m[r][c] << " ";
        cout << "\n";
    }
}

Move<char>* MisereUI::get_move(Player<char>* p) {
    if (p->get_type() == PlayerType::COMPUTER) {
        auto* ai = dynamic_cast<MisereAIPlayer*>(p);
        return ai->get_best_move();
    }

    cout << p->get_name() << " enter row and column:\n";
    int r = read_int_in_range(0,2);
    int c = read_int_in_range(0,2);
    return new Move<char>(r, c, p->get_symbol());
}
