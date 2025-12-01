#include "BoardGame_Classes.h"
#include "Diamond_TTT.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <algorithm>
using namespace std;

// ==================== BOARD IMPLEMENTATION ====================

/**
 * @brief Checks if the given cell lies inside the diamond shape.
 */
bool DiamondBoard::is_valid_cell(int r, int c) const {
    return (abs(r - 2) + abs(c - 2) <= 2);
}

/**
 * @brief Constructs a diamond 5x5 board.
 * Playable cells are '.', blocked cells are '#'.
 */
DiamondBoard::DiamondBoard() : Board<char>(5, 5) {
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            board[r][c] = is_valid_cell(r, c) ? '.' : '#';
    n_moves = 0;
}

/**
 * @brief Attempts to execute a move at (r,c) with symbol sym.
 */
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

/**
 * @brief Returns directions where player has a sequence of length.
 */
vector<int> DiamondBoard::get_directions(Player<char>* player, int length) const {
    char S = player->get_symbol();
    return get_directions_for_symbol(S, length);
}

/**
 * @brief Returns directions where given symbol has a sequence.
 */
vector<int> DiamondBoard::get_directions_for_symbol(char S, int length) const {
    vector<int> dirs;

    // horizontal
    for (int r = 0; r < 5; r++)
        for (int c = 0; c <= 5 - length; c++) {
            bool ok = true;
            for (int k = 0; k < length; k++)
                if (!is_valid_cell(r, c+k) || board[r][c+k] != S) ok = false;
            if (ok) dirs.push_back(1);
        }

    // vertical
    for (int c = 0; c < 5; c++)
        for (int r = 0; r <= 5 - length; r++) {
            bool ok = true;
            for (int k = 0; k < length; k++)
                if (!is_valid_cell(r+k, c) || board[r+k][c] != S) ok = false;
            if (ok) dirs.push_back(2);
        }

    // diagonal ↘
    for (int r = 0; r <= 5 - length; r++)
        for (int c = 0; c <= 5 - length; c++) {
            bool ok = true;
            for (int k = 0; k < length; k++)
                if (!is_valid_cell(r+k, c+k) || board[r+k][c+k] != S) ok = false;
            if (ok) dirs.push_back(3);
        }

    // diagonal ↙
    for (int r = 0; r <= 5 - length; r++)
        for (int c = length-1; c < 5; c++) {
            bool ok = true;
            for (int k = 0; k < length; k++)
                if (!is_valid_cell(r+k, c-k) || board[r+k][c-k] != S) ok = false;
            if (ok) dirs.push_back(4);
        }

    return dirs;
}

/**
 * @brief Win requires both 3-in-row AND 4-in-row in different directions.
 */
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

/**
 * @brief Draw occurs when all 13 playable cells are used.
 */
bool DiamondBoard::is_draw(Player<char>* /*player*/) {
    return n_moves == 13;
}

/**
 * @brief Game over when win or draw.
 */
bool DiamondBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

/**
 * @brief Returns all playable empty cells.
 */
vector<pair<int, int>> DiamondBoard::get_available_moves() const {
    vector<pair<int, int>> moves;
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            if (is_valid_cell(r, c) && board[r][c] == '.')
                moves.push_back({r, c});
    return moves;
}

/**
 * @brief Checks if a symbol has winning pattern.
 */
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

/**
 * @brief AI helper: place symbol on board.
 */
void DiamondBoard::make_move(int r, int c, char symbol) {
    board[r][c] = symbol;
    n_moves++;
}

/**
 * @brief AI helper: undo symbol placement.
 */
void DiamondBoard::undo_move(int r, int c) {
    board[r][c] = '.';
    n_moves--;
}

// ==================== AI PLAYER IMPLEMENTATION ====================

/**
 * @brief Constructs AI player.
 */
DiamondAIPlayer::DiamondAIPlayer(string name, char symbol) 
    : Player<char>(name, symbol, PlayerType::COMPUTER) {}

/**
 * @brief Evaluates board state heuristically.
 */
int DiamondAIPlayer::evaluate_board(DiamondBoard* board, char aiSymbol, char humanSymbol) {
    if (board->check_win_state(aiSymbol)) return 1000;
    if (board->check_win_state(humanSymbol)) return -1000;

    int score = 0;

    auto ai3 = board->get_directions_for_symbol(aiSymbol, 3);
    auto ai4 = board->get_directions_for_symbol(aiSymbol, 4);
    auto h3  = board->get_directions_for_symbol(humanSymbol, 3);
    auto h4  = board->get_directions_for_symbol(humanSymbol, 4);

    score += ai3.size() * 10;
    score += ai4.size() * 15;
    score -= h3.size() * 10;
    score -= h4.size() * 15;

    if (!ai3.empty() && !ai4.empty()) score += 50;
    if (!h3.empty() && !h4.empty()) score -= 50;

    return score;
}

/**
 * @brief Minimax with alpha-beta pruning.
 */
int DiamondAIPlayer::minimax(DiamondBoard* board, int depth, bool isMaximizing,
                             char aiSymbol, char humanSymbol, int alpha, int beta) {
    if (board->check_win_state(aiSymbol)) return 1000 - depth;
    if (board->check_win_state(humanSymbol)) return -1000 + depth;
    if (board->get_n_moves() == 13 || depth == MAX_DEPTH)
        return evaluate_board(board, aiSymbol, humanSymbol);

    auto moves = board->get_available_moves();

    if (isMaximizing) {
        int best = numeric_limits<int>::min();
        for (auto& m : moves) {
            board->make_move(m.first, m.second, aiSymbol);
            int score = minimax(board, depth+1, false, aiSymbol, humanSymbol, alpha, beta);
            board->undo_move(m.first, m.second);
            best = max(best, score);
            alpha = max(alpha, score);
            if (beta <= alpha) break;
        }
        return best;
    } else {
        int worst = numeric_limits<int>::max();
        for (auto& m : moves) {
            board->make_move(m.first, m.second, humanSymbol);
            int score = minimax(board, depth+1, true, aiSymbol, humanSymbol, alpha, beta);
            board->undo_move(m.first, m.second);
            worst = min(worst, score);
            beta = min(beta, score);
            if (beta <= alpha) break;
        }
        return worst;
    }
}

/**
 * @brief Computes best move using minimax algorithm.
 */
Move<char>* DiamondAIPlayer::get_best_move() {
    auto* board = dynamic_cast<DiamondBoard*>(boardPtr);
    if (!board) return nullptr;

    auto moves = board->get_available_moves();
    if (moves.empty()) return nullptr;

    char ai = symbol;
    char human = (symbol == 'X' ? 'O' : 'X');

    int bestScore = numeric_limits<int>::min();
    pair<int, int> bestMove = moves[0];

    cout << "\n" << name << " (AI) is thinking..." << endl;

    for (auto& m : moves) {
        board->make_move(m.first, m.second, ai);
        int score = minimax(board, 0, false, ai, human,
                            numeric_limits<int>::min(), numeric_limits<int>::max());
        board->undo_move(m.first, m.second);

        if (score > bestScore) {
            bestScore = score;
            bestMove = m;
        }
    }

    cout << name << " chooses (" << bestMove.first << ", " << bestMove.second << ")\n";
    return new Move<char>(bestMove.first, bestMove.second, ai);
}

// ==================== UI IMPLEMENTATION ====================

/**
 * @brief Initializes UI with welcome message.
 */
DiamondUI::DiamondUI()
    : UI<char>("Welcome to Diamond Tic-Tac-Toe!", 2) {}

/**
 * @brief Creates either human or AI players.
 */
Player<char>* DiamondUI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::COMPUTER ? "AI" : "Human")
         << " Player " << name << " (" << symbol << ")\n";

    if (type == PlayerType::COMPUTER)
        return new DiamondAIPlayer(name, symbol);

    return new Player<char>(name, symbol, type);
}

/**
 * @brief Retrieves move from player (human or AI).
 */
Move<char>* DiamondUI::get_move(Player<char>* player) {
    if (auto ai = dynamic_cast<DiamondAIPlayer*>(player))
        return ai->get_best_move();

    int r, c;
    cout << "\n" << player->get_name() << ", enter your move (row col): ";

    while (!(cin >> r >> c)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid! enter row col: ";
    }

    return new Move<char>(r, c, player->get_symbol());
}
