#include <iostream>
#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Misere_TICTAKTOE.h"
using namespace std;

/// @brief 
/// @return 
int main() {
    int choice;
    cout << "============================\n";
    cout << "   Welcome to Board Games!\n";
    cout << "============================\n";
    cout << "1. XO DEMO\n";
    cout << "2. Misere Tic-Tac-Toe\n";
    cout << "============================\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    
    if (choice == 1) {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* ui = new XO_UI();
        Board<char>* board = new X_O_Board();
        Player<char>** players = ui->setup_players();

        GameManager<char> game(board, players, ui);
        game.run();

        delete board;
        delete[] players;
        delete ui;
        
    }
    else if (choice == 2) {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* ui = new MisereUI();
        Board<char>* board = new MisereBoard();
        Player<char>** players = ui->setup_players();

        GameManager<char> game(board, players, ui);
        game.run();

        delete board;
        delete[] players;
        delete ui;
    }
    else {
        cout << "Exiting the program. Goodbye!\n";
    }

    return 0;
}
