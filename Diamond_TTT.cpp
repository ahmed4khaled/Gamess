#include "BoardGame_Classes.h"
#include "Diamond_TTT.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <algorithm>
using namespace std;

// ==================== BOARD IMPLEMENTATION ====================

bool DiamondBoard::is_valid_cell(int r, int c) const {
    return (abs(r - 2) + abs(c - 2) <= 2);
}

DiamondBoard::DiamondBoard() : Board<char>(5, 5) {
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            board[r][c] = is_valid_cell(r, c) ? '.' : '#';
    n_moves = 0;
}

bool DiamondBoard::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char sym = toupper(move->get_symbol());

    if (r < 0 || r >= 5 || c < 0 || c >= 5) return false;
    if (!is_valid_cell(r, c)) return false;
    if (board[r][c] != '.') return false;

    board[r][c] = sym;
    n_moves++;
    return true;
}

vector<int> DiamondBoard::get_directions(Player<char>* player, int length) const {
    char S = player->get_symbol();
    return get_directions_for_symbol(S, length);
}

vector<int> DiamondBoard::get_directions_for_symbol(char S, int length) const {
    vector<int> dirs;

    // horizontal (direction 1)
    for (int r = 0; r < 5; r++)
        for (int c = 0; c <= 5 - length; c++) {
            bool ok = true;
            for (int k = 0; k < length; k++)
                if (!is_valid_cell(r, c+k) || board[r][c+k] != S) ok = false;
            if (ok) dirs.push_back(1);
        }

    // vertical (direction 2)
    for (int c = 0; c < 5; c++)
        for (int r = 0; r <= 5 - length; r++) {
            bool ok = true;
            for (int k = 0; k < length; k++)
                if (!is_valid_cell(r+k, c) || board[r+k][c] != S) ok = false;
            if (ok) dirs.push_back(2);
        }

    // diagonal ↘ (direction 3)
    for (int r = 0; r <= 5 - length; r++)
        for (int c = 0; c <= 5 - length; c++) {
            bool ok = true;
            for (int k = 0; k < length; k++)
                if (!is_valid_cell(r+k, c+k) || board[r+k][c+k] != S) ok = false;
            if (ok) dirs.push_back(3);
        }

    // diagonal ↙ (direction 4)
    for (int r = 0; r <= 5 - length; r++)
        for (int c = length-1; c < 5; c++) {
            bool ok = true;
            for (int k = 0; k < length; k++)
                if (!is_valid_cell(r+k, c-k) || board[r+k][c-k] != S) ok = false;
            if (ok) dirs.push_back(4);
        }

    return dirs;
}

bool DiamondBoard::is_win(Player<char>* player) {
    if (!player) return false;

    vector<int> d3 = get_directions(player, 3);
    vector<int> d4 = get_directions(player, 4);

    if (d3.empty() || d4.empty())
        return false;

    for (int a : d3)
        for (int b : d4)
            if (a != b) return true;

    return false;
}

bool DiamondBoard::is_draw(Player<char>* /*player*/) {
    return n_moves == 13;
}

bool DiamondBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

vector<pair<int, int>> DiamondBoard::get_available_moves() const {
    vector<pair<int, int>> moves;
    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 5; c++) {
            if (is_valid_cell(r, c) && board[r][c] == '.') {
                moves.push_back({r, c});
            }
        }
    }
    return moves;
}

bool DiamondBoard::check_win_state(char symbol) const {
    vector<int> d3 = get_directions_for_symbol(symbol, 3);
    vector<int> d4 = get_directions_for_symbol(symbol, 4);

    if (d3.empty() || d4.empty())
        return false;

    for (int a : d3)
        for (int b : d4)
            if (a != b) return true;

    return false;
}

void DiamondBoard::make_move(int r, int c, char symbol) {
    board[r][c] = symbol;
    n_moves++;
}

void DiamondBoard::undo_move(int r, int c) {
    board[r][c] = '.';
    n_moves--;
}

// ==================== AI PLAYER IMPLEMENTATION ====================

DiamondAIPlayer::DiamondAIPlayer(string name, char symbol) 
    : Player<char>(name, symbol, PlayerType::COMPUTER) {}

int DiamondAIPlayer::evaluate_board(DiamondBoard* board, char aiSymbol, char humanSymbol) {
    // Check for win/loss
    if (board->check_win_state(aiSymbol)) {
        return 1000;
    }
    if (board->check_win_state(humanSymbol)) {
        return -1000;
    }
    
    // Heuristic evaluation
    int score = 0;
    
    // Count sequences for AI
    vector<int> ai_3 = board->get_directions_for_symbol(aiSymbol, 3);
    vector<int> ai_4 = board->get_directions_for_symbol(aiSymbol, 4);
    
    // Count sequences for Human
    vector<int> human_3 = board->get_directions_for_symbol(humanSymbol, 3);
    vector<int> human_4 = board->get_directions_for_symbol(humanSymbol, 4);
    
    // Scoring strategy:
    // Having both 3-in-a-row and 4-in-a-row is valuable
    score += ai_3.size() * 10;
    score += ai_4.size() * 15;
    score -= human_3.size() * 10;
    score -= human_4.size() * 15;
    
    // Bonus for having multiple different directions
    if (!ai_3.empty() && !ai_4.empty()) {
        score += 50;
    }
    if (!human_3.empty() && !human_4.empty()) {
        score -= 50;
    }
    
    return score;
}

int DiamondAIPlayer::minimax(DiamondBoard* board, int depth, bool isMaximizing, 
                              char aiSymbol, char humanSymbol, int alpha, int beta) {
    // Terminal conditions
    if (board->check_win_state(aiSymbol)) {
        return 1000 - depth;
    }
    if (board->check_win_state(humanSymbol)) {
        return -1000 + depth;
    }
    if (board->get_n_moves() == 13 || depth == MAX_DEPTH) {
        return evaluate_board(board, aiSymbol, humanSymbol);
    }
    
    vector<pair<int, int>> moves = board->get_available_moves();
    
    if (isMaximizing) {
        int maxScore = numeric_limits<int>::min();
        
        for (auto& move : moves) {
            board->make_move(move.first, move.second, aiSymbol);
            int score = minimax(board, depth + 1, false, aiSymbol, humanSymbol, alpha, beta);
            board->undo_move(move.first, move.second);
            
            maxScore = max(maxScore, score);
            alpha = max(alpha, score);
            
            if (beta <= alpha) {
                break; // Beta cutoff
            }
        }
        return maxScore;
    } else {
        int minScore = numeric_limits<int>::max();
        
        for (auto& move : moves) {
            board->make_move(move.first, move.second, humanSymbol);
            int score = minimax(board, depth + 1, true, aiSymbol, humanSymbol, alpha, beta);
            board->undo_move(move.first, move.second);
            
            minScore = min(minScore, score);
            beta = min(beta, score);
            
            if (beta <= alpha) {
                break; // Alpha cutoff
            }
        }
        return minScore;
    }
}

Move<char>* DiamondAIPlayer::get_best_move() {
    DiamondBoard* board = dynamic_cast<DiamondBoard*>(boardPtr);
    if (!board) return nullptr;
    
    vector<pair<int, int>> moves = board->get_available_moves();
    if (moves.empty()) return nullptr;
    
    char aiSymbol = this->symbol;
    char humanSymbol = (aiSymbol == 'X') ? 'O' : 'X';
    
    int bestScore = numeric_limits<int>::min();
    pair<int, int> bestMove = moves[0];
    
    cout << "\n" << this->name << " (AI) is thinking..." << flush;
    
    for (auto& move : moves) {
        board->make_move(move.first, move.second, aiSymbol);
        int score = minimax(board, 0, false, aiSymbol, humanSymbol, 
                           numeric_limits<int>::min(), numeric_limits<int>::max());
        board->undo_move(move.first, move.second);
        
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    
    cout << "\n" << this->name << " chooses position (" << bestMove.first << ", " << bestMove.second << ")\n" << endl;
    return new Move<char>(bestMove.first, bestMove.second, aiSymbol);
}

// ==================== UI IMPLEMENTATION ====================

DiamondUI::DiamondUI()
    : UI<char>("Welcome to Diamond Tic-Tac-Toe!", 2) {}

Player<char>* DiamondUI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::COMPUTER) {
        cout << "Creating AI Player " << name << " (" << symbol << ")\n";
        return new DiamondAIPlayer(name, symbol);
    } else {
        cout << "Creating Human Player " << name << " (" << symbol << ")\n";
        return new Player<char>(name, symbol, type);
    }
}

Move<char>* DiamondUI::get_move(Player<char>* player) {
    // Check if this is an AI player
    DiamondAIPlayer* aiPlayer = dynamic_cast<DiamondAIPlayer*>(player);
    if (aiPlayer) {
        Move<char>* move = aiPlayer->get_best_move();
        return move;
    }
    
    // Human player
    int r, c;
    cout << "\n" << player->get_name() << ", enter your move (row col): ";
    
    while (!(cin >> r >> c)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter row and column numbers: ";
    }

    return new Move<char>(r, c, player->get_symbol());
}