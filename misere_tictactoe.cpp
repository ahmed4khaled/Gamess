#include <iostream>
#include <iomanip>
#include <cctype>
#include <limits>
#include <vector>
#include "Misere_TICTAKTOE.h"
using namespace std;

// ---------------- MisereBoard implementation ----------------

MisereBoard::MisereBoard() : Board<char>(3, 3) {
    for (auto &r : board)
        for (auto &c : r)
            c = blank_symbol;
    n_moves = 0;
}

bool MisereBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns))
    {
        if (mark == 0) { // undo
            // Only undo if currently not blank
            if (board[x][y] != blank_symbol) {
                n_moves--;
                board[x][y] = blank_symbol;
                return true;
            }
            return false;
        } else {
            if (board[x][y] == blank_symbol) {
                n_moves++;
                board[x][y] = toupper(mark);
                return true;
            }
        }
    }
    return false;
}

bool MisereBoard::has_three_in_row(char s) const {
    // rows
    for (int i = 0; i < rows; ++i)
        if (board[i][0] == s && board[i][1] == s && board[i][2] == s)
            return true;

    // cols
    for (int j = 0; j < columns; ++j)
        if (board[0][j] == s && board[1][j] == s && board[2][j] == s)
            return true;

    // diagonals
    if (board[0][0] == s && board[1][1] == s && board[2][2] == s) return true;
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;

    return false;
}

bool MisereBoard::is_win(Player<char>* /*player*/) {
    // In Misère, creating a line is a loss, not a win.
    return false;
}

bool MisereBoard::is_lose(Player<char>* player) {
    char s = player->get_symbol();
    s = toupper(s);
    return has_three_in_row(s);
}

bool MisereBoard::is_draw(Player<char>* /*player*/) {
    bool x_three = has_three_in_row('X');
    bool o_three = has_three_in_row('O');
    return (n_moves == rows * columns) && !x_three && !o_three;
}

bool MisereBoard::game_is_over(Player<char>* player) {
    return is_lose(player) || is_draw(player);
}

// ---------------- MisereUI implementation ----------------

MisereUI::MisereUI() : UI<char>("Welcome to Misère Tic-Tac-Toe (avoid making 3-in-a-row)", 3) {}

Player<char>* MisereUI::create_player(std::string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

// Minimx/backtracking implementation for Misère Tic-Tac-Toe
// Score semantics:
//   + (positive) => good for AI (opponent more likely to lose)
//   - (negative) => bad for AI (AI likely to lose)
//   0 => draw
int MisereUI::minimax(MisereBoard* board, char aiSymbol, char curSymbol, int depth, bool isMax) {
    // create temp player for curSymbol to test is_lose
    Player<char> tempPlayer("temp", curSymbol, PlayerType::COMPUTER);
    Player<char> aiPlayer("aiTemp", aiSymbol, PlayerType::COMPUTER);

    // If the player who just moved created a three-in-a-row, they lose.
    // But here we are being called for the side to move now. We will check terminal states
    // immediately after making a move in the recursion. So at the start of this call,
    // we consider that no immediate loss has just occurred (handled by caller).
    // However we still check for full board draw:
    if (board->is_draw(&tempPlayer)) {
        return 0; // draw
    }

    // We'll generate all possible moves for current player curSymbol
    int best;
    if (isMax) best = numeric_limits<int>::min();
    else best = numeric_limits<int>::max();

    // Iterate cells
    for (int r = 0; r < board->get_rows(); ++r) {
        for (int c = 0; c < board->get_columns(); ++c) {
            // try placing at r,c if empty
            // We have no direct "get_cell" API, so check by attempting to place and reading success
            Move<char> tryMove(r, c, curSymbol);
            bool placed = board->update_board(&tryMove);
            if (!placed) continue; // cell occupied or invalid

            // After placing, if curSymbol created 3-in-a-row => curSymbol loses immediately
            Player<char> placedPlayer("p", curSymbol, PlayerType::COMPUTER);
            if (board->is_lose(&placedPlayer)) {
                // curSymbol lost by making a line; so this branch yields:
                int score;
                if (curSymbol == aiSymbol) {
                    // AI just made losing move
                    score = -10 + depth; // penalize, prefer later losses (hence +depth)
                } else {
                    // Opponent just made losing move -> good for AI
                    score = 10 - depth;
                }

                // undo move
                Move<char> undoMove(r, c, 0);
                board->update_board(&undoMove);

                // evaluate for minimax
                if (isMax) best = max(best, score);
                else best = min(best, score);

                // continue to next cell
                continue;
            }

            // Not terminal -> if board full now -> draw
            if (board->is_draw(&placedPlayer)) {
                // undo
                Move<char> undoMove(r, c, 0);
                board->update_board(&undoMove);

                if (isMax) best = max(best, 0);
                else best = min(best, 0);
                continue;
            }

            // Recurse: next player moves
            char nextSymbol = (curSymbol == 'X') ? 'O' : 'X';
            int val = minimax(board, aiSymbol, nextSymbol, depth + 1, !isMax);

            // undo move
            Move<char> undoMove(r, c, 0);
            board->update_board(&undoMove);

            if (isMax) best = max(best, val);
            else best = min(best, val);
        }
    }

    // If no moves possible (shouldn't happen because draw check above), return draw
    if (best == numeric_limits<int>::min()) return 0;
    if (best == numeric_limits<int>::max()) return 0;
    return best;
}

Move<char>* MisereUI::find_best_move(MisereBoard* board, char aiSymbol) {
    int bestVal = numeric_limits<int>::min();
    Move<char>* bestMove = nullptr;

    for (int r = 0; r < board->get_rows(); ++r) {
        for (int c = 0; c < board->get_columns(); ++c) {
            Move<char> mv(r, c, aiSymbol);
            bool placed = board->update_board(&mv);
            if (!placed) continue;

            Player<char> placedPlayer("p", aiSymbol, PlayerType::COMPUTER);
            int score;

            // If AI immediately creates a 3-in-row -> AI loses -> very bad
            if (board->is_lose(&placedPlayer)) {
                score = -10; // immediate loss
            } else if (board->is_draw(&placedPlayer)) {
                score = 0;
            } else {
                // opponent moves next
                char next = (aiSymbol == 'X') ? 'O' : 'X';
                score = minimax(board, aiSymbol, next, 1, false);
            }

            // undo
            Move<char> undo(r, c, 0);
            board->update_board(&undo);

            if (score > bestVal) {
                bestVal = score;
                if (bestMove) delete bestMove;
                bestMove = new Move<char>(r, c, aiSymbol);
            }
        }
    }

    // If no move found (shouldn't happen), pick first empty cell
    if (!bestMove) {
        for (int r = 0; r < board->get_rows(); ++r) {
            for (int c = 0; c < board->get_columns(); ++c) {
                Move<char> mv(r, c, aiSymbol);
                if (board->update_board(&mv)) {
                    // undo immediately and return this
                    Move<char> undo(r, c, 0);
                    board->update_board(&undo);
                    return new Move<char>(r, c, aiSymbol);
                }
            }
        }
    }

    return bestMove;
}

Move<char>* MisereUI::get_move(Player<char>* player) {
    int x = 0, y = 0;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    } else {
        // Computer: use backtracking/minimax to find best move
        MisereBoard* board = dynamic_cast<MisereBoard*>(player->get_board_ptr());
        if (!board) {
            // fallback: random move
            int r = rand() % 3;
            int c = rand() % 3;
            return new Move<char>(r, c, player->get_symbol());
        }

        Move<char>* best = find_best_move(board, toupper(player->get_symbol()));
        if (best) return best;

        // fallback random
        for (int r = 0; r < board->get_rows(); ++r)
            for (int c = 0; c < board->get_columns(); ++c) {
                Move<char> mv(r, c, player->get_symbol());
                if (board->update_board(&mv)) {
                    Move<char> undo(r, c, 0);
                    board->update_board(&undo);
                    return new Move<char>(r, c, player->get_symbol());
                }
            }

        // final fallback (shouldn't happen)
        return new Move<char>(0, 0, player->get_symbol());
    }
}
