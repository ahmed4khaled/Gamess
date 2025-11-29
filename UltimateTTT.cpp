#include "UltimateTTT.h"
#include <iostream>
#include <random>
#include <chrono>

using namespace std;

UltimateTTTBoard::UltimateTTTBoard()
    : Board<char>(9, 9),
      mainBoardWinners(3, vector<char>(3, '-')),
      nextRequired({-1, -1})
{
    board.assign(9, vector<char>(9, '-'));
    n_moves = 0;
}

vector<vector<char>> UltimateTTTBoard::get_board_matrix() const {
    return board;
}

pair<int,int> UltimateTTTBoard::get_next_required() const {
    return nextRequired;
}

bool UltimateTTTBoard::isSmallClosed(int br, int bc) const {
    return mainBoardWinners[br][bc] != '-';
}

char UltimateTTTBoard::checkSmallWinner(int br, int bc) const {
    int r0 = br * 3;
    int c0 = bc * 3;

    // rows + cols
    for (int i = 0; i < 3; i++) {
        // row
        if (board[r0+i][c0] != '-' &&
            board[r0+i][c0] == board[r0+i][c0+1] &&
            board[r0+i][c0+1] == board[r0+i][c0+2])
            return board[r0+i][c0];

        // col
        if (board[r0][c0+i] != '-' &&
            board[r0][c0+i] == board[r0+1][c0+i] &&
            board[r0+1][c0+i] == board[r0+2][c0+i])
            return board[r0][c0+i];
    }

    // diag 1
    if (board[r0][c0] != '-' &&
        board[r0][c0] == board[r0+1][c0+1] &&
        board[r0+1][c0+1] == board[r0+2][c0+2])
        return board[r0][c0];

    // diag 2
    if (board[r0][c0+2] != '-' &&
        board[r0][c0+2] == board[r0+1][c0+1] &&
        board[r0+1][c0+1] == board[r0+2][c0])
        return board[r0][c0+2];

    // full? → tie
    bool full = true;
    for (int i = 0; i < 3 && full; i++)
        for (int j = 0; j < 3; j++)
            if (board[r0+i][c0+j] == '-') full = false;

    return full ? 'T' : '-';
}

bool UltimateTTTBoard::is_cell_allowed(int full_r, int full_c) const {
    if (full_r < 0 || full_r >= 9 || full_c < 0 || full_c >= 9) return false;
    if (board[full_r][full_c] != '-') return false;

    int br = full_r / 3;
    int bc = full_c / 3;

    // free move (small board forced is closed)
    if (nextRequired.first == -1)
        return !isSmallClosed(br, bc);

    // must play in specific small board
    if (br == nextRequired.first && bc == nextRequired.second)
        return !isSmallClosed(br, bc);

    return false;
}

bool UltimateTTTBoard::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char sym = move->get_symbol();

    if (!is_cell_allowed(r, c)) return false;

    board[r][c] = sym;
    n_moves++;

    int br = r / 3;
    int bc = c / 3;

    // update small board winner
    if (mainBoardWinners[br][bc] == '-') {
        char w = checkSmallWinner(br, bc);
        if (w != '-')
            mainBoardWinners[br][bc] = w;  // 'X' or 'O' or 'T'
    }

    // determine next required board
    int nr = r % 3;
    int nc = c % 3;

    if (isSmallClosed(nr, nc))
        nextRequired = {-1, -1}; // free
    else
        nextRequired = {nr, nc};

    return true;
}

bool UltimateTTTBoard::checkMainWin(char s) {
    // rows
    for (int i = 0; i < 3; i++)
        if (mainBoardWinners[i][0] == s &&
            mainBoardWinners[i][1] == s &&
            mainBoardWinners[i][2] == s)
            return true;

    // cols
    for (int j = 0; j < 3; j++)
        if (mainBoardWinners[0][j] == s &&
            mainBoardWinners[1][j] == s &&
            mainBoardWinners[2][j] == s)
            return true;

    // diags
    if (mainBoardWinners[0][0] == s &&
        mainBoardWinners[1][1] == s &&
        mainBoardWinners[2][2] == s)
        return true;

    if (mainBoardWinners[0][2] == s &&
        mainBoardWinners[1][1] == s &&
        mainBoardWinners[2][0] == s)
        return true;

    return false;
}

bool UltimateTTTBoard::is_win(Player<char>* p) {
    return checkMainWin(p->get_symbol());
}

bool UltimateTTTBoard::is_lose(Player<char>* p) {
    char opp = (p->get_symbol() == 'X' ? 'O' : 'X');
    return checkMainWin(opp);
}

bool UltimateTTTBoard::is_draw(Player<char>* p) {
    bool allClosed = true;
    for (int i = 0; i < 3 && allClosed; i++)
        for (int j = 0; j < 3; j++)
            if (mainBoardWinners[i][j] == '-') allClosed = false;

    return allClosed && !checkMainWin('X') && !checkMainWin('O');
}

bool UltimateTTTBoard::game_is_over(Player<char>* p) {
    return is_win(p) || is_lose(p) || is_draw(p);
}

/* ===================== UI ====================== */

UltimateTTT_UI::UltimateTTT_UI() : UI<char>(3) {}

static vector<pair<int,int>> collect_valid(UltimateTTTBoard* b) {
    vector<pair<int,int>> out;
    auto mat = b->get_board_matrix();

    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            if (mat[r][c] == '-' && b->is_cell_allowed(r, c))
                out.push_back({r, c});

    return out;
}

Move<char>* UltimateTTT_UI::get_move(Player<char>* p) {
    auto* b = dynamic_cast<UltimateTTTBoard*>(p->get_board_ptr());
    if (!b) return new Move<char>(0, 0, p->get_symbol());

    // COMPUTER: random
    if (p->get_type() == PlayerType::COMPUTER) {
        auto moves = collect_valid(b);
        if (moves.empty()) return new Move<char>(0,0,p->get_symbol());

        static mt19937_64 rng(
            (unsigned) chrono::high_resolution_clock::now().time_since_epoch().count()
        );
        uniform_int_distribution<size_t> dist(0, moves.size()-1);
        auto pick = moves[dist(rng)];

        return new Move<char>(pick.first, pick.second, p->get_symbol());
    }

    // HUMAN
    while (true) {
        auto req = b->get_next_required();
        if (req.first == -1)
            cout << p->get_name() << " (" << p->get_symbol() << ") — You may play in ANY open small board.\n";
        else
            cout << p->get_name() << " (" << p->get_symbol()
                 << ") — You MUST play in small board (" << req.first << "," << req.second << ").\n";

        int br, bc, r, c;

        cout << "Enter small board row (0-2): ";
        cin >> br;
        cout << "Enter small board col (0-2): ";
        cin >> bc;
        cout << "Enter cell row (0-2): ";
        cin >> r;
        cout << "Enter cell col (0-2): ";
        cin >> c;

        if (br<0||br>2||bc<0||bc>2||r<0||r>2||c<0||c>2) {
            cout << "Invalid input.\n";
            continue;
        }

        int full_r = br*3 + r;
        int full_c = bc*3 + c;

        if (!b->is_cell_allowed(full_r, full_c)) {
            cout << "Cell not allowed.\n";
            continue;
        }

        return new Move<char>(full_r, full_c, p->get_symbol());
    }
}
