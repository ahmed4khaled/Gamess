/**
 * @file Numerical_TTT.cpp
 * @brief Implementation of Numerical Tic-Tac-Toe board & UI logic.
 *
 * Game Summary:
 * - Board is 3×3.
 * - Player 1 uses ODD numbers {1,3,5,7,9}.
 * - Player 2 uses EVEN numbers {2,4,6,8}.
 * - A number cannot be repeated after being used.
 * - Win condition: Any row/column/diagonal summing to 15.
 * - Draw: Board full with no winning line.
 */

#include "Numerical_TicTacToe.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

/* ============================================================
 *                 BOARD IMPLEMENTATION
 * ============================================================ */

/**
 * @brief Constructs a Numerical Tic-Tac-Toe board.
 *
 * Initializes:
 * - 3×3 board filled with `blank_symbol` (0)
 * - Player 1 available numbers = {1,3,5,7,9}
 * - Player 2 available numbers = {2,4,6,8}
 */
Numerical_TicTacToe_Board::Numerical_TicTacToe_Board() : Board<int>(3, 3) {
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            board[r][c] = blank_symbol;

    player1_available = {1, 3, 5, 7, 9};
    player2_available = {2, 4, 6, 8};

    n_moves = 0;
}

/**
 * @brief Attempts to place a number on the board.
 *
 * Conditions for valid move:
 * - Cell is inside the board.
 * - Cell is empty.
 * - Number belongs to correct player set (odd/even).
 * - Number has not been used before.
 *
 * @param move Pointer to Move object (row, col, number).
 * @return true if the move was applied, false if invalid.
 */
bool Numerical_TicTacToe_Board::update_board(Move<int>* move) {
    if (!move) return false;

    int x = move->get_x();
    int y = move->get_y();
    int number = move->get_symbol();

    // Bounds check
    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    // Must be empty
    if (board[x][y] != blank_symbol)
        return false;

    // Must be positive
    if (number <= 0)
        return false;

    // Choose correct available set
    set<int>* available_set = (number % 2 == 1)
                                ? &player1_available
                                : &player2_available;

    // Ensure number is unused
    if (available_set->find(number) == available_set->end())
        return false;

    // Apply move
    board[x][y] = number;
    available_set->erase(number);
    ++n_moves;

    return true;
}

/**
 * @brief Checks whether any row/column/diagonal sums to 15.
 *
 * Only checks sums when all three cells are non-zero.
 *
 * @return true if a 15-sum exists.
 */
bool Numerical_TicTacToe_Board::is_win(Player<int>* /*player*/) {
    // Check rows
    for (int r = 0; r < 3; ++r)
        if (board[r][0] && board[r][1] && board[r][2])
            if (board[r][0] + board[r][1] + board[r][2] == 15)
                return true;

    // Check columns
    for (int c = 0; c < 3; ++c)
        if (board[0][c] && board[1][c] && board[2][c])
            if (board[0][c] + board[1][c] + board[2][c] == 15)
                return true;

    // Main diagonal
    if (board[0][0] && board[1][1] && board[2][2])
        if (board[0][0] + board[1][1] + board[2][2] == 15)
            return true;

    // Anti diagonal
    if (board[0][2] && board[1][1] && board[2][0])
        if (board[0][2] + board[1][1] + board[2][0] == 15)
            return true;

    return false;
}
/**
 * @brief Player loses if OPPONENT forms a line summing to 15.
 *
 * Numerical TicTacToe considers 'is_lose' as the opposite of is_win.
 */
bool Numerical_TicTacToe_Board::is_lose(Player<int>* player) {
    (void)player; 
    return false; // Loss is handled by game_is_over + is_win for opponent
}

/**
 * @brief Draw if board is full AND no 15-sum line.
 * @return true if draw.
 */
bool Numerical_TicTacToe_Board::is_draw(Player<int>* /*player*/) {
    if (n_moves < rows * columns) return false;
    return !is_win(nullptr);
}

/**
 * @brief Game ends if win or draw.
 * @return true if game is finished.
 */
bool Numerical_TicTacToe_Board::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}

/**
 * @brief Returns available unused numbers for a player.
 *
 * @param player_num 1 or 2
 * @return const reference to that player's available set
 */
const set<int>& Numerical_TicTacToe_Board::get_available_numbers(int player_num) const {
    return (player_num == 1) ? player1_available : player2_available;
}

/**
 * @brief Checks if cell is empty.
 */
bool Numerical_TicTacToe_Board::is_cell_empty(int x, int y) const {
    if (x < 0 || x >= rows || y < 0 || y >= columns) return false;
    return board[x][y] == blank_symbol;
}

/* ============================================================
 *                 UI IMPLEMENTATION
 * ============================================================ */

/**
 * @brief Constructs UI with welcome message.
 */
Numerical_TicTacToe_UI::Numerical_TicTacToe_UI()
    : UI<int>("Welcome to FCAI Numerical Tic-Tac-Toe Game", 3)
{
    srand(static_cast<unsigned>(time(nullptr)));
}

/**
 * @brief Creates human or AI player.
 *
 * Symbol meaning:
 * - 1 → odd-number player
 * - 2 → even-number player
 */
Player<int>* Numerical_TicTacToe_UI::create_player(string& name, int symbol, PlayerType type) {
    return new Player<int>(name, symbol, type);
}

/**
 * @brief Displays Numerical Tic-Tac-Toe rules.
 */
void Numerical_TicTacToe_UI::display_rules() const {
    std::cout << "\n========== NUMERICAL TIC-TAC-TOE ==========\n";
    std::cout << "Rules:\n";
    std::cout << "- Player 1 uses odd numbers: {1,3,5,7,9}\n";
    std::cout << "- Player 2 uses even numbers: {2,4,6,8}\n";
    std::cout << "- Numbers cannot be reused.\n";
    std::cout << "- Win by making a line (row, column, diagonal) summing to 15.\n";
    std::cout << "===========================================\n\n";
}

/**
 * @brief Requests move from human or generates AI move.
 *
 * Human:
 *   Enter: row col number
 *
 * Computer:
 *   Picks:
 *     - random available number from correct set
 *     - random empty cell
 *
 * @return Move pointer
 */
Move<int>* Numerical_TicTacToe_UI::get_move(Player<int>* p) {
    if (!p) return nullptr;

    auto* board_ptr = dynamic_cast<Numerical_TicTacToe_Board*>(p->get_board_ptr());
    if (!board_ptr) return nullptr;

    // === Human logic ===
    if (p->get_type() == PlayerType::HUMAN) {
        int x, y, num;
        cout << "\n" << p->get_name()
             << " - enter your move (row col number): ";
        cin >> x >> y >> num;
        return new Move<int>(x, y, num);
    }

    // === Computer logic ===
    int playerSymbol = p->get_symbol(); // 1 => odd, 2 => even
    const set<int>& avail = board_ptr->get_available_numbers(playerSymbol);

    // Collect empty cells
    vector<pair<int,int>> empties;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (board_ptr->is_cell_empty(r, c))
                empties.emplace_back(r, c);

    if (empties.empty() || avail.empty())
        return new Move<int>(0, 0, 0); // fallback

    // Random choice of empty cell
    auto [rx, ry] = empties[rand() % empties.size()];

    // Random choice of available number
    int index = rand() % avail.size();
    auto it = avail.begin();
    advance(it, index);
    int chosenNumber = *it;

    cout << "Computer plays number " << chosenNumber
         << " at (" << rx << "," << ry << ")\n";

    return new Move<int>(rx, ry, chosenNumber);
}
