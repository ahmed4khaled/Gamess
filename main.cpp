#include <iostream>
#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Misere_TICTAKTOE.h"
#include "Numerical_TicTacToe.h"
#include "Diamond_TTT.h"
using namespace std;

int main() {
    while (true) {

        int choice;
        cout << "============================\n";
        cout << "   Welcome to FCAI Board Games!\n";
        cout << "============================\n";
        cout << "1.  Normal XO \n";
        cout << "============================\n";
        cout << "2. Misere Tic-Tac-Toe\n";
        cout << "============================\n";
        cout << "3. Numerical Tic-Tac-Toe\n";
        cout << "============================\n";
        cout << "4. Diamond Tic-Tac-Toe\n";
        cout << "============================\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 5) {
            cout << "Exiting the program. Goodbye!\n";
            return 0;
        }

        // ============================
        // 1. XO DEMO
        // ============================
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

        // ============================
        // 2. Misere Tic Tac Toe
        // ============================
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

        // ============================
// 3. Numerical Tic Tac Toe
// ============================
else if (choice == 3) {
    srand(static_cast<unsigned int>(time(0)));

    UI<int>* ui = new Numerical_TicTacToe_UI();
    Board<int>* board = new Numerical_TicTacToe_Board();
    Player<int>** players = ui->setup_players();

    GameManager<int> game(board, players, ui);
    game.run();

    delete board;
    delete[] players;
    delete ui;
}

        // ============================
        // 4. Diamond Tic Tac Toe
        // ============================
        else if (choice == 4) {
            srand(static_cast<unsigned int>(time(0)));

            UI<char>* ui = new DiamondUI();
            Board<char>* board = new DiamondBoard();
            Player<char>** players = ui->setup_players();

            GameManager<char> game(board, players, ui);
            game.run();

            delete board;
            delete[] players;
            delete ui;
        }

        else {
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}
