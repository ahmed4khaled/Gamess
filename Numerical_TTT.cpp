//--------------------------------------- IMPLEMENTATION

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Numerical_TicTacToe.h"

using namespace std;

//--------------------------------------- Numerical_TicTacToe_Board Implementation

Numerical_TicTacToe_Board::Numerical_TicTacToe_Board() : Board<int>(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : this->board)
        for (auto& cell : row)
            cell = blank_symbol;
    
    // Initialize available numbers
    player1_available = {1, 3, 5, 7, 9}; // Odd numbers for Player 1
    player2_available = {2, 4, 6, 8};    // Even numbers for Player 2
}

bool Numerical_TicTacToe_Board::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int number = move->get_symbol();

    // Validate move boundaries
    if (x < 0 || x >= this->rows || y < 0 || y >= this->columns) {
        return false;
    }

    // Check if cell is already occupied
    if (this->board[x][y] != blank_symbol) {
        return false;
    }

    // Determine which player is making the move based on odd/even number
    set<int>* available_set = nullptr;
    if (number % 2 == 1) { // Odd number - Player 1
        available_set = &player1_available;
    } else if (number % 2 == 0 && number > 0) { // Even number - Player 2
        available_set = &player2_available;
    } else {
        return false; // Invalid number
    }

    // Check if the number is available
    if (available_set->find(number) == available_set->end()) {
        return false; // Number already used or invalid
    }

    // Apply move
    this->board[x][y] = number;
    available_set->erase(number);
    this->n_moves++;
    
    return true;
}

bool Numerical_TicTacToe_Board::is_win(Player<int>* player) {
    // Check all rows
    for (int i = 0; i < this->rows; ++i) {
        if (this->board[i][0] != blank_symbol && 
            this->board[i][1] != blank_symbol && 
            this->board[i][2] != blank_symbol) {
            if (this->board[i][0] + this->board[i][1] + this->board[i][2] == 15) {
                return true;
            }
        }
    }

    // Check all columns
    for (int j = 0; j < this->columns; ++j) {
        if (this->board[0][j] != blank_symbol && 
            this->board[1][j] != blank_symbol && 
            this->board[2][j] != blank_symbol) {
            if (this->board[0][j] + this->board[1][j] + this->board[2][j] == 15) {
                return true;
            }
        }
    }

    // Check main diagonal (top-left to bottom-right)
    if (this->board[0][0] != blank_symbol && 
        this->board[1][1] != blank_symbol && 
        this->board[2][2] != blank_symbol) {
        if (this->board[0][0] + this->board[1][1] + this->board[2][2] == 15) {
            return true;
        }
    }

    // Check anti-diagonal (top-right to bottom-left)
    if (this->board[0][2] != blank_symbol && 
        this->board[1][1] != blank_symbol && 
        this->board[2][0] != blank_symbol) {
        if (this->board[0][2] + this->board[1][1] + this->board[2][0] == 15) {
            return true;
        }
    }

    return false;
}

bool Numerical_TicTacToe_Board::is_draw(Player<int>* player) {
    // Game is a draw if all cells are filled and no one has won
    return (this->n_moves == 9 && !is_win(player));
}

bool Numerical_TicTacToe_Board::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}

const set<int>& Numerical_TicTacToe_Board::get_available_numbers(int player_num) const {
    if (player_num == 1) {
        return player1_available;
    } else {
        return player2_available;
    }
}

bool Numerical_TicTacToe_Board::is_cell_empty(int x, int y) const {
    if (x < 0 || x >= this->rows || y < 0 || y >= this->columns) {
        return false;
    }
    return this->board[x][y] == blank_symbol;
}

//--------------------------------------- Numerical_TicTacToe_UI Implementation

Numerical_TicTacToe_UI::Numerical_TicTacToe_UI() 
    : UI<int>("Welcome to FCAI Numerical Tic-Tac-Toe Game", 3) {}

Player<int>* Numerical_TicTacToe_UI::create_player(string& name, int symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (Player " << symbol 
         << " - " << (symbol == 1 ? "Odd numbers" : "Even numbers") << ")\n";
    
    cout << "[DEBUG] PlayerType value: " << static_cast<int>(type) << endl;

    Player<int>* p = new Player<int>(name, symbol, type);
    
    cout << "[DEBUG] Created player type check: " << (p->get_type() == PlayerType::HUMAN ? "HUMAN" : "COMPUTER") << endl;
    
    return p;
}

Move<int>* Numerical_TicTacToe_UI::get_move(Player<int>* player) {
    int x, y, number;
    Numerical_TicTacToe_Board* board_ptr = 
        dynamic_cast<Numerical_TicTacToe_Board*>(player->get_board_ptr());
    
    cout << "\n=== GET_MOVE CALLED ===" << endl;
    cout << "[DEBUG] Player name: " << player->get_name() << endl;
    cout << "[DEBUG] Player symbol: " << player->get_symbol() << endl;
    cout << "[DEBUG] Player type: " << (player->get_type() == PlayerType::HUMAN ? "HUMAN" : 
                                       player->get_type() == PlayerType::COMPUTER ? "COMPUTER" : "UNKNOWN") << endl;
    cout << "[DEBUG] PlayerType enum value: " << static_cast<int>(player->get_type()) << endl;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "[DEBUG] Entering HUMAN branch" << endl;
        // Display available numbers
        const set<int>& available = board_ptr->get_available_numbers(player->get_symbol());
        cout << "\nPlayer " << player->get_name() << " - Available numbers: ";
        for (int num : available) {
            cout << num << " ";
        }
        cout << endl;

        // Get move from human player
        cout << "Enter your move (row column number): ";
        cin >> x >> y >> number;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        cout << "[DEBUG] Entering COMPUTER branch" << endl;
        // Computer makes a random valid move
        const set<int>& available = board_ptr->get_available_numbers(player->get_symbol());
        
        if (available.empty()) {
            cout << "[DEBUG] No available numbers!" << endl;
            return nullptr; // No available numbers
        }

        // Pick a random available number
        auto it = available.begin();
        advance(it, rand() % available.size());
        number = *it;

        // Find a random empty cell
        do {
            x = rand() % board_ptr->get_rows();
            y = rand() % board_ptr->get_columns();
        } while (!board_ptr->is_cell_empty(x, y));

        cout << "\nComputer " << player->get_name() << " plays: " << number 
             << " at position (" << x << ", " << y << ")\n";
    }
    else {
        cout << "[DEBUG] Entering ELSE branch - UNKNOWN PLAYER TYPE!" << endl;
    }
    
    return new Move<int>(x, y, number);
}

Player<int>** Numerical_TicTacToe_UI::setup_players() {
    Player<int>** players = new Player<int>*[2];
    
    cout << "\n=== SETUP_PLAYERS CALLED ===" << endl;
    
    string name1;
    cout << "Enter Player 1 Name: ";
    cin >> name1;

    cout << "Choose Player 1 type:\n1. Human\n2. Computer\n";
    int t1;
    cin >> t1;
    PlayerType type1 = (t1 == 1 ? PlayerType::HUMAN : PlayerType::COMPUTER);
    
    cout << "[DEBUG] Player 1 type selected: " << (type1 == PlayerType::HUMAN ? "HUMAN" : "COMPUTER") << endl;

    // Player 1 uses ODD numbers (symbol = 1)
    players[0] = create_player(name1, 1, type1);

    string name2;
    cout << "\nEnter Player 2 Name: ";
    cin >> name2;

    cout << "Choose Player 2 type:\n1. Human\n2. Computer\n";
    int t2;
    cin >> t2;
    PlayerType type2 = (t2 == 1 ? PlayerType::HUMAN : PlayerType::COMPUTER);
    
    cout << "[DEBUG] Player 2 type selected: " << (type2 == PlayerType::HUMAN ? "HUMAN" : "COMPUTER") << endl;

    // Player 2 uses EVEN numbers (symbol = 2)
    players[1] = create_player(name2, 2, type2);

    return players;
}