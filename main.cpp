#include <iostream>
#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Misere_TICTAKTOE.h"
#include "Numerical_TicTacToe.h"
#include "Diamond_TTT.h"
#include "FiveByFive_Board.h"
#include "FiveByFive_UI.h"
#include "Word_Board.h"
#include "Word_UI.h"
#include "Obstacles_Board.h"
#include "Obstacles_UI.h"
#include "UltimateTTT.h"
#include "4x4_MovingTTT.h"

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
        cout << "5. 5x5 Tic-Tac-Toe \n";
        cout << "============================\n";
        cout << "6. Word Game \n";
        cout << "============================\n";
        cout << "7. Obstacles Game \n";
        cout << "============================\n";
        cout << "8. Ultimate Tic-Tac-Toe\n";
        cout << "============================\n";
        cout << "9. 4 x 4 Moving Tic-Tac-Toe\n";
        cout << "============================\n";
        cout << "10. Exit\n";
        cout << "============================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 10) {
            cout << "Exiting the program. Goodbye!\n";
            return 0;
        }

        // ============================
        // 1. XO Game
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
        // ============================
        // 5. 5x5 Tic Tac Toe
        // ============================

   else if (choice == 5) {
    srand(static_cast<unsigned int>(time(0)));
    UI<char>* ui = new FiveByFive_UI();
    Board<char>* board = new FiveByFive_Board();
    Player<char>** players = ui->setup_players(); // uses UI default to create X and O

    GameManager<char> game(board, players, ui);
    game.run();

    delete board;
    delete[] players;
    delete ui;
}
        // ============================
        // 6. Word Game
        // ============================


else if (choice == 6)
{
     srand(static_cast<unsigned int>(time(0)));
    UI<char>* ui = new Word_UI();
    Board<char>* board = new Word_Board("dic.txt", '-');
    Player<char>** players = ui->setup_players();

    GameManager<char> game(board, players, ui);
    game.run();

    delete board;
    delete[] players;
    delete ui;

}
        // ============================
        // 7. Obstacles Game
        // ============================ 
else if (choice == 7) {
    srand(static_cast<unsigned int>(time(0)));
    UI<char>* ui = new Obstacles_UI();
    Board<char>* board = new Obstacles_Board('.', '#');
    Player<char>** players = ui->setup_players();
    GameManager<char> game(board, players, ui);
    game.run();

    delete board;
    delete[] players;
    delete ui;
}

        // ============================
        // 8. Ultimate Tic-Tac-Toe
        // ============================
        else if (choice == 8) {
    UI<char>* ui = new UltimateTTT_UI();
    Board<char>* board = new UltimateTTTBoard();
    Player<char>** players = ui->setup_players();

    GameManager<char> game(board, players, ui);
    game.run();

    delete board;
    delete[] players;
    delete ui;
}
        // ============================
        // 9. 4x4 Moving Tic Tac Toe
        else if (choice == 9) {
    srand(static_cast<unsigned int>(time(0)));
    UI<char>* ui = new MovingTTT4x4_UI();
    Board<char>* board = new MovingTTT4x4_Board();
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
