#include "Misere_TicTacToe.h"
#include <iostream>
#include <limits>
#include <cctype>

using namespace std;

MisereBoard::MisereBoard() : Board(3,3) {
    for (auto &r : board) for (auto &c : r) c = blank_symbol;
}

bool MisereBoard::update_board(Move<char>* move) {
    int x = move->get_x(), y = move->get_y();
    char s = move->get_symbol();
    if (x < 0 || x >= rows || y < 0 || y >= columns) return false;
    if (board[x][y] != blank_symbol) return false;
    board[x][y] = toupper(s);
    n_moves++;
    return true;
}

bool MisereBoard::has_three(char sym) const {
    auto three=[&](char a,char b,char c){ return a==sym && b==sym && c==sym; };
    for (int i=0;i<3;i++)
        if (three(board[i][0], board[i][1], board[i][2])) return true;
    for (int i=0;i<3;i++)
        if (three(board[0][i], board[1][i], board[2][i])) return true;
    if (three(board[0][0], board[1][1], board[2][2])) return true;
    if (three(board[0][2], board[1][1], board[2][0])) return true;
    return false;
}

bool MisereBoard::is_lose(Player<char>* player) {
    // A player loses if they themselves have 3-in-row (misère rule)
    return has_three(player->get_symbol());
}

bool MisereBoard::is_draw(Player<char>* player) {
    return (n_moves == 9 && !has_three('X') && !has_three('O'));
}

bool MisereBoard::game_is_over(Player<char>* player) {
    return is_lose(player) || is_draw(player);
}

/* ----------------- MisereUI (with Minimax) ----------------- */

MisereUI::MisereUI() : UI<char>("Welcome to Misere Tic-Tac-Toe (avoid 3-in-row)!", 3) {}

Player<char>* MisereUI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating player: " << name << " (" << symbol << ") Type: "
         << (type == PlayerType::HUMAN ? "Human" : "Computer") << "\n";
    return new Player<char>(name, symbol, type);
}

// helper: check 3-in-row on arbitrary board matrix
bool MisereUI::board_has_three(const vector<vector<char>>& boardm, char sym) const {
    auto three=[&](char a,char b,char c){ return a==sym && b==sym && c==sym; };
    for (int i=0;i<3;i++)
        if (three(boardm[i][0], boardm[i][1], boardm[i][2])) return true;
    for (int i=0;i<3;i++)
        if (three(boardm[0][i], boardm[1][i], boardm[2][i])) return true;
    if (three(boardm[0][0], boardm[1][1], boardm[2][2])) return true;
    if (three(boardm[0][2], boardm[1][1], boardm[2][0])) return true;
    return false;
}

// minimax for misère: returns +1 if the maximizing player can force a win (i.e., force opponent to make 3-in-row),
// -1 if maximizing player will be forced to make 3-in-row, 0 for draw.
int MisereUI::minimax(vector<vector<char>>& boardm, char turnSym, char maxSym, int depth) {
    // terminal checks
    if (board_has_three(boardm, maxSym)) {
        // maxSym already has 3 -> maxSym loses in misere, so return -1 for maxSym
        return -1;
    }
    if (board_has_three(boardm, (maxSym == 'X') ? 'O' : 'X')) {
        // opponent has 3 -> maxSym wins
        return +1;
    }
    // count empties
    int empties = 0;
    for (auto &r: boardm) for (auto c: r) if (c=='.') empties++;
    if (empties == 0) return 0; // draw

    // maximizing or minimizing?
    bool isMax = (turnSym == maxSym);
    int best = isMax ? numeric_limits<int>::min() : numeric_limits<int>::max();

    for (int i=0;i<3;i++) for (int j=0;j<3;j++) {
        if (boardm[i][j] != '.') continue;
        boardm[i][j] = turnSym;
        int score = minimax(boardm, (turnSym=='X'?'O':'X'), maxSym, depth+1);
        boardm[i][j] = '.';
        if (isMax) {
            best = max(best, score);
            // alpha-beta would help but not necessary for 3x3
        } else {
            best = min(best, score);
        }
    }
    return best;
}

// choose best move for AI using minimax
pair<int,int> MisereUI::best_move_minimax(char mySym) {
    vector<vector<char>> m = { {'.','.','.'},{'.','.','.'},{'.','.','.'} };
    // try to copy current board from Game board via UI display is not available here; instead use board pointer of player.
    // We'll require callers to pass Player that has boardPtr; instead we'll search through static global? Simpler:
    // The GameManager will call get_move with player who has boardPtr set. So here we will obtain the board from player's pointer
    // (see get_move below). This helper isn't used alone.

    return {0,0}; // placeholder: not used directly
}

Move<char>* MisereUI::get_move(Player<char>* player) {
    int x=-1,y=-1;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " enter your move x y (0..2): ";
        cin >> x >> y;
        return new Move<char>(x,y,player->get_symbol());
    }

    // COMPUTER: run Minimax on the current board
    Board<char>* bptr = player->get_board_ptr();
    if (!bptr) {
        // fallback random
        x = rand()%3; y = rand()%3;
        return new Move<char>(x,y,player->get_symbol());
    }

    // copy board
    vector<vector<char>> boardm = bptr->get_board_matrix();

    int bestScore = numeric_limits<int>::min();
    pair<int,int> bestMove = {-1,-1};
    char mySym = player->get_symbol();
    for (int i=0;i<3;i++) for (int j=0;j<3;j++) {
        if (boardm[i][j] != '.') continue;
        boardm[i][j] = mySym;
        // If this immediate move creates three for me, it's instant loss, score = -1
        int score;
        if (board_has_three(boardm, mySym)) {
            score = -1;
        } else {
            score = minimax(boardm, (mySym=='X'?'O':'X'), mySym, 0);
        }
        boardm[i][j] = '.';
        if (score > bestScore) {
            bestScore = score;
            bestMove = {i,j};
        }
    }

    if (bestMove.first == -1) {
        // fallback random
        do {
            x = rand()%3; y = rand()%3;
        } while (bptr->get_board_matrix()[x][y] != '.');
        return new Move<char>(x,y,player->get_symbol());
    }

    return new Move<char>(bestMove.first, bestMove.second, player->get_symbol());
}
