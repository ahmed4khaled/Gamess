#include "SUS_TicTacToe.h"
#include <iostream>
#include <limits>
#include <iomanip>


/**
* @brief SUS game (3 X 3) Board
*/

SUSBoard::SUSBoard() : Board(3, 3) {
    score[0] = score[1] = 0;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';
}

bool SUSBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char s = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3)
        return false;

    if (board[x][y] != ' ')
        return false;

    board[x][y] = s;
    n_moves++;

    check_sus(x, y, s);
    return true;
}

void SUSBoard::check_sus(int x, int y, char s) {
    int dx[8] = { 0,0,1,-1,1,1,-1,-1 };
    int dy[8] = { 1,-1,0,0,1,-1,1,-1 };
    
    int playerIndex = (n_moves % 2 == 1) ? 0 : 1;

    for (int i = 0; i < 8; i++) {
        int x1 = x + dx[i];
        int y1 = y + dy[i];
        int x2 = x + 2 * dx[i];
        int y2 = y + 2 * dy[i];

        if (x2 >= 0 && x2 < 3 && y2 >= 0 && y2 < 3) {
            if (board[x][y] == 'S' &&
                board[x1][y1] == 'U' &&
                board[x2][y2] == 'S') {

                score[playerIndex]++;
            }
        }
    }
}

int SUSBoard::get_score(int i) {
    return score[i];
}

bool SUSBoard::is_win(Player<char>* p) {
	/**
    * @brief ensure scores are updated
    */
    check_sus(0, 0, ' ');
    int playerIndex = (n_moves % 2 == 1) ? 0 : 1;
	cout << "Player " << p->get_name() << " Score: " << score[playerIndex] << endl;
    return score[playerIndex] >= 3;
}

bool SUSBoard::is_lose(Player<char>* p) {
    return false;
}

bool SUSBoard::is_draw(Player<char>* p) {
    return n_moves == 9;
}

bool SUSBoard::game_is_over(Player<char>* p) {
    return is_draw(p);
}

/**
* @brief SUS game user interface 
*/
SUS_UI::SUS_UI() : UI<char>(3) {}

char SUS_UI::ask_letter(Player<char>* p) {
    char c;
    cout << p->get_name() << " choose your letter (S or U): ";
    cin >> c;
    while (c != 'S' && c != 'U') {
        cout << "Invalid! Enter S or U only: ";
        cin >> c;
    }
    playerLetter[p] = c;
    return c;
}

Move<char>* SUS_UI::smart_computer_move(Player<char>* p) {
    SUSBoard* b = dynamic_cast<SUSBoard*>(p->get_board_ptr());
    auto mat = b->get_board_matrix();
    char my = playerLetter[p];
    char opp = (my == 'S') ? 'U' : 'S';

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (mat[i][j] == ' ')
                return new Move<char>(i, j, my);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (mat[i][j] == ' ')
                return new Move<char>(i, j, my);

    int x, y;
    do {
        x = rand() % 3;
        y = rand() % 3;
    } while (mat[x][y] != ' ');

    return new Move<char>(x, y, my);
}


Move<char>* SUS_UI::get_move(Player<char>* p) {

    if (playerLetter.find(p) == playerLetter.end()) {
        if (p->get_type() == PlayerType::COMPUTER) {
            playerLetter[p] = (rand() % 2 == 0) ? 'S' : 'U';
            cout << p->get_name() << " chose letter: "
                << playerLetter[p] << endl;
        }
        else {
            ask_letter(p);
        }
    }

    char fixedLetter = playerLetter[p];

    if (p->get_type() == PlayerType::COMPUTER) {
        return smart_computer_move(p);
    }

    int x, y;
    cout << p->get_name() << " enter row and column: ";
    cin >> x >> y;

    cout << "Your fixed letter is: " << fixedLetter << endl;

    return new Move<char>(x, y, fixedLetter);
}

SUS_UI::~SUS_UI() {}