#include "SUS_TicTacToe.h"
#include <iostream>
#include <limits>
#include <iomanip>

using namespace std;

// ================================
// SUS_Board Implementation
// ================================

SUS_Board::SUS_Board() : player1_score(0), player2_score(0) {
    rows = 3;
    columns = 3;
    board = new char* [rows];
    for (int i = 0; i < rows; i++) {
        board[i] = new char[columns];
        for (int j = 0; j < columns; j++) {
            board[i][j] = ' ';
        }
    }
    n_moves = 0;
}

bool SUS_Board::is_sus_sequence(char c1, char c2, char c3) {
    return (c1 == 'S' && c2 == 'U' && c3 == 'S');
}

pair<int, int> SUS_Board::count_all_sus_sequences() {
    int p1_count = 0;
    int p2_count = 0;

    // Check all horizontal lines
    for (int i = 0; i < 3; i++) {
        if (is_sus_sequence(board[i][0], board[i][1], board[i][2])) {
            // Determine which player made the last move in this sequence
            // For simplicity, we'll track this during gameplay
            if (n_moves % 2 == 1) p1_count++;
            else p2_count++;
        }
    }

    // Check all vertical lines
    for (int j = 0; j < 3; j++) {
        if (is_sus_sequence(board[0][j], board[1][j], board[2][j])) {
            if (n_moves % 2 == 1) p1_count++;
            else p2_count++;
        }
    }

    // Check diagonal (top-left to bottom-right)
    if (is_sus_sequence(board[0][0], board[1][1], board[2][2])) {
        if (n_moves % 2 == 1) p1_count++;
        else p2_count++;
    }

    // Check diagonal (top-right to bottom-left)
    if (is_sus_sequence(board[0][2], board[1][1], board[2][0])) {
        if (n_moves % 2 == 1) p1_count++;
        else p2_count++;
    }

    return { p1_count, p2_count };
}

bool SUS_Board::update_board(int x, int y, char symbol) {
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;
    }
    if (board[x][y] != ' ') {
        return false;
    }
    if (symbol != 'S' && symbol != 'U') {
        return false;
    }

    board[x][y] = symbol;
    n_moves++;

    // Update scores
    auto scores = count_all_sus_sequences();
    player1_score = scores.first;
    player2_score = scores.second;

    return true;
}

void SUS_Board::display_board() {
    cout << "\n";
    cout << "     0   1   2\n";
    cout << "   +---+---+---+\n";
    for (int i = 0; i < rows; i++) {
        cout << " " << i << " |";
        for (int j = 0; j < columns; j++) {
            cout << " " << board[i][j] << " |";
        }
        cout << "\n   +---+---+---+\n";
    }
    cout << "\n";
}

pair<int, int> SUS_Board::get_scores() {
    return { player1_score, player2_score };
}

bool SUS_Board::is_win() {
    // Game doesn't end on win, only when board is full
    return false;
}

bool SUS_Board::is_draw() {
    if (n_moves < 9) return false;
    return player1_score == player2_score;
}

bool SUS_Board::game_is_over() {
    return n_moves >= 9;
}

void SUS_Board::undo_move(int x, int y) {
    if (board[x][y] != ' ') {
        board[x][y] = ' ';
        n_moves--;

        // Recalculate scores
        auto scores = count_all_sus_sequences();
        player1_score = scores.first;
        player2_score = scores.second;
    }
}

// ================================
// SUS_Player Implementation
// ================================

SUS_Player::SUS_Player(string n, char symbol) : Player<char>(n, symbol) {
    this->name = n;
    this->symbol = symbol;
}

void SUS_Player::getmove(int& x, int& y) {
    char letter;

    cout << "\n" << name << "'s turn (" << symbol << ")\n";
    cout << "Enter your move (row col letter): ";

    while (true) {
        cin >> x >> y >> letter;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again: ";
            continue;
        }

        letter = toupper(letter);
        if (letter != 'S' && letter != 'U') {
            cout << "Invalid letter! Use S or U. Try again: ";
            continue;
        }

        // Store the chosen letter in symbol temporarily
        this->symbol = letter;
        break;
    }
}

// ================================
// SUS_AI_Player Implementation
// ================================

SUS_AI_Player::SUS_AI_Player(char symbol) : Player<char>("AI", symbol) {
    this->name = "AI Player";
    this->symbol = symbol;
}

int SUS_AI_Player::evaluate_board(SUS_Board* board) {
    auto scores = board->get_scores();
    // AI is player 2, so maximize player2_score - player1_score
    return scores.second - scores.first;
}

int SUS_AI_Player::minimax(SUS_Board* board, int depth, bool is_maximizing, int alpha, int beta) {
    // Base case: game over or depth limit
    if (board->game_is_over() || depth == 0) {
        return evaluate_board(board);
    }

    if (is_maximizing) {
        int max_eval = numeric_limits<int>::min();

        // Try all possible moves
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board->board[i][j] == ' ') {
                    // Try both S and U
                    for (char letter : {'S', 'U'}) {
                        board->update_board(i, j, letter);
                        int eval = minimax(board, depth - 1, false, alpha, beta);
                        board->undo_move(i, j);

                        max_eval = max(max_eval, eval);
                        alpha = max(alpha, eval);

                        if (beta <= alpha) {
                            break; // Beta cutoff
                        }
                    }
                }
            }
        }
        return max_eval;
    }
    else {
        int min_eval = numeric_limits<int>::max();

        // Try all possible moves
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board->board[i][j] == ' ') {
                    // Try both S and U
                    for (char letter : {'S', 'U'}) {
                        board->update_board(i, j, letter);
                        int eval = minimax(board, depth - 1, true, alpha, beta);
                        board->undo_move(i, j);

                        min_eval = min(min_eval, eval);
                        beta = min(beta, eval);

                        if (beta <= alpha) {
                            break; // Alpha cutoff
                        }
                    }
                }
            }
        }
        return min_eval;
    }
}

pair<int, int> SUS_AI_Player::find_best_move(SUS_Board* board) {
    int best_score = numeric_limits<int>::min();
    pair<int, int> best_move = { -1, -1 };
    char best_letter = 'S';

    // Try all possible moves
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board->board[i][j] == ' ') {
                // Try both S and U
                for (char letter : {'S', 'U'}) {
                    board->update_board(i, j, letter);

                    // Use minimax with depth 6 for good performance
                    int score = minimax(board, 6, false,
                        numeric_limits<int>::min(),
                        numeric_limits<int>::max());

                    board->undo_move(i, j);

                    if (score > best_score) {
                        best_score = score;
                        best_move = { i, j };
                        best_letter = letter;
                    }
                }
            }
        }
    }

    this->symbol = best_letter;
    return best_move;
}

void SUS_AI_Player::getmove(int& x, int& y) {
    cout << "\n" << name << " is thinking...\n";

    SUS_Board* board_ptr = dynamic_cast<SUS_Board*>(this->boardPtr);
    if (board_ptr) {
        auto move = find_best_move(board_ptr);
        x = move.first;
        y = move.second;

        cout << "AI chooses: row " << x << ", col " << y << ", letter " << symbol << "\n";
    }
}

// ================================
// SUS_UI Implementation
// ================================

SUS_UI::SUS_UI() {}

Player<char>** SUS_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    string name;
    int choice;

    cout << "\n=================================\n";
    cout << "   SUS Tic-Tac-Toe Setup\n";
    cout << "=================================\n";
    cout << "Rules:\n";
    cout << "- Players take turns placing S or U\n";
    cout << "- Form 'S-U-S' sequences to score\n";
    cout << "- Most sequences wins!\n";
    cout << "=================================\n\n";

    // Player 1
    cout << "Player 1 - Enter name: ";
    cin >> name;
    players[0] = new SUS_Player(name, 'X');

    // Player 2
    cout << "\nPlayer 2 options:\n";
    cout << "1. Human\n";
    cout << "2. AI (with Minimax & Backtracking)\n";
    cout << "Choose: ";
    cin >> choice;

    if (choice == 1) {
        cout << "Player 2 - Enter name: ";
        cin >> name;
        players[1] = new SUS_Player(name, 'O');
    }
    else {
        players[1] = new SUS_AI_Player('O');
    }

    return players;
}

void SUS_UI::display_winner(Player<char>* winner) {
    cout << "\n=================================\n";
    if (winner) {
        cout << "   🎉 " << winner->getname() << " WINS! 🎉\n";
    }
    else {
        cout << "   It's a DRAW!\n";
    }
    cout << "=================================\n\n";
}

void SUS_UI::display_scores(pair<int, int> scores,
    Player<char>* player1,
    Player<char>* player2) {
    cout << "\n--- Current Scores ---\n";
    cout << player1->getname() << ": " << scores.first << " SUS sequences\n";
    cout << player2->getname() << ": " << scores.second << " SUS sequences\n";
    cout << "----------------------\n";
}