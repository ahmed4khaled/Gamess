#include <iostream>
#include "XO_Classes.h"
#include "BoardGame_Classes.h"
using namespace std;

int main() {
    int choice;
    cout << "============================\n";
    cout << "   Welcome to Board Games!\n";
    cout << "============================\n";
    cout << "1. Play XO Game\n";
    cout << "2. Exit\n";
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
    else {
        cout << "Goodbye!\n";
    }

    return 0;
}
