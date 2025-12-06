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
#include "Pyramid_TTT.h"
#include "4x4_MovingTTT.h"
#include "Memory_TTT.h"
#include "SUS_TicTacToe.h"
#include "Connect4.h"
#include "InfinityTicTacToe.h"


using namespace std;

/**
 * @brief Entry point for FCAI Board Games application.
 *
 * Displays a menu with multiple Tic-Tac-Toe variations and other grid games.
 * Based on user selection, initializes the correct Board, UI, and Players,
 * and executes the game via GameManager.
 */
int main() {
    while (true) {

        int choice;

        // --- Main Menu --- //
        cout << "============================\n";
        cout << "   Welcome to FCAI Board Games!\n";
        cout << "============================\n";
        cout << "Please select a game to play:\n";
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
        cout << "10. Pyramid Tic-Tac-Toe\n";
        cout << "============================\n";
        cout << "11. Memory Tic-Tac-Toe\n";
        cout << "============================\n";
        cout << "12. SUS Tic-Tac-Toe\n";
        cout << "============================\n";
        cout << "13. Connect Four\n";
        cout << "============================\n";
        cout << "14. Infinity Tic-Tac-Toe\n";
        cout << "============================\n";
        cout << "15. Exit\n";
        cout << "============================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        

        // ============================
        // 1. Normal XO
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
            Player<char>** players = ui->setup_players();

            GameManager<char> game(board, players, ui);
            game.run();

            delete board;
            delete[] players;
            delete ui;
        }

        // ============================
        // 6. Word Game
        // ============================
        else if (choice == 6) {
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
        // 8. Ultimate Tic Tac Toe
        // ============================
        else if (choice == 8) {
            srand(static_cast<unsigned int>(time(0)));

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
        // 9. 4×4 Moving Tic Tac Toe
        // ============================
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
        // ============================
        // 10. Pyramid Tic Tac Toe
        // ============================
        else if (choice == 10) {
    srand(static_cast<unsigned int>(time(0)));

    UI<char>* ui = new Pyramid_UI();
    Board<char>* board = new PyramidBoard();
    Player<char>** players = ui->setup_players();

    GameManager<char> game(board, players, ui);
    game.run();

    delete board;
    delete[] players;
    delete ui;
}
// ============================                         
// 11. Memory Tic Tac Toe
// ============================
else if (choice == 11) {
    srand(static_cast<unsigned int>(time(0)));

    UI<char>* ui = new Memory_UI();
    Board<char>* board = new MemoryBoard();
    Player<char>** players = ui->setup_players();

    GameManager<char> game(board, players, ui);
    game.run();

    delete board;
    delete[] players;
    delete ui;
}

// ============================
// 12. SUS Tic Tac Toe
// ============================
else if (choice == 12) {
    srand(static_cast<unsigned int>(time(0)));
    UI<char>* ui = new SUS_UI();
    Board<char>* board = new SUSBoard();
    Player<char>** players = ui->setup_players();
    GameManager<char> game(board, players, ui);
    game.run();
    delete board;
    delete[] players;
    delete ui;
}

// ============================
// 13.  Connect Four
// ============================
else if (choice == 13) {
    srand(static_cast<unsigned int>(time(0)));
    UI<char>* ui = new Connect4_UI() ;
    Board<char>* board = new Connect4_Board();
    Player<char>** players = ui->setup_players();
    GameManager<char> game(board, players, ui);
    game.run();
    delete board;
    delete[] players;
    delete ui;
}
///////////////////////////
// 14. Infinity Tic Tac Toe
///////////////////////////
else if (choice == 14) {
    srand(static_cast<unsigned int>(time(0)));

    InfinityBoard<char>* board = new InfinityBoard<char>();
    InfinityUI<char>* ui = new InfinityUI<char>();
    Player<char>** players = ui->setup_players();

    GameManager<char> game(board, players, ui);
    game.run();

    delete board;
    delete ui;
    delete players[0];
    delete players[1];
    delete[] players;

        }


// Exit condition
else if (choice == 15) {
            cout << "Exiting the program. Goodbye!\n";
            return 0;
        }
else {
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}
