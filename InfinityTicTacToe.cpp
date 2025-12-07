/**
 * @file InfinityTicTacToe.cpp
 * @brief Implementations for Infinity Tic-Tac-Toe templates and explicit instantiation for char.
 * NOTE: template definitions are placed here and explicitly instantiated for `char`.
 */

#include "InfinityTicTacToe.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>

 /**
 * @brief Seed the random number generator once.
 */
static bool _infty_seeded = []() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    return true;
    }();

/**
* @brief InfinityBoard implementations
*/

template <typename T>
InfinityBoard<T>::InfinityBoard() : Board<T>(3, 3) {
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            this->board[r][c] = static_cast<T>(' ');
    this->n_moves = 0;
}

template <typename T>
bool InfinityBoard<T>::update_board(Move<T>* move) {
    int x = move->get_x();
    int y = move->get_y();
    T sym = move->get_symbol();

    /**
	* @brief check move coordinates
    */
    if (x < 0 || x >= 3 || y < 0 || y >= 3)
        return false;

    /**
    * @brief chech cell is empty
    */
    if (this->board[x][y] != static_cast<T>(' '))
        return false;

    /**
	* @brief Place the symbol on the board and increment move count.
    */
    this->board[x][y] = sym;
    this->n_moves++;


    if (sym == static_cast<T>('X')) {
        /**
        * @brief record move for player X
        */
        history_X.push({ x, y });  

        if (history_X.size() > 3) {
            auto oldest = history_X.front();
            history_X.pop();
            this->board[oldest.first][oldest.second] = static_cast<T>(' ');
        }
    }
    else { 
       /**
	   * @brief record move for player O
       */
        history_O.push({ x, y });   

        /**
		* @brief If O has made more than 3 moves, remove the oldest move.
        */
        if (history_O.size() > 3) {
            auto oldest = history_O.front();
            history_O.pop();
            this->board[oldest.first][oldest.second] = static_cast<T>(' ');
        }
    }

    return true;
}

template <typename T>
bool InfinityBoard<T>::is_win(Player<T>* player) {
    T s = player->get_symbol();

    /**
	* @brief Check rows
    */
    for (int r = 0; r < 3; ++r)
        if (this->board[r][0] == s && this->board[r][1] == s && this->board[r][2] == s)
            return true;

    /**
	* @brief Check Columns
    */    
    for (int c = 0; c < 3; ++c)
        if (this->board[0][c] == s && this->board[1][c] == s && this->board[2][c] == s)
            return true;

    /**
    * @brief Check diagonals
    */
    if (this->board[0][0] == s && 
        this->board[1][1] == s && 
        this->board[2][2] == s) 
        return true;

    if (this->board[0][2] == s && 
        this->board[1][1] == s && 
        this->board[2][0] == s) 
        return true;

    return false;
}

template <typename T>
bool InfinityBoard<T>::is_lose(Player<T>*) {
    // Loss is determined by opponent's win; not used here.
    return false;
}

template <typename T>
bool InfinityBoard<T>::is_draw(Player<T>*) {
/**
* @brief Draw detection: if more than 50 moves have been made without a win, declare draw.
*/
    return this->n_moves > 50;
}

template <typename T>
bool InfinityBoard<T>::game_is_over(Player<T>* player) {
    return is_win(player) || is_draw(player);
}

/**
* @brief InfinityPlayer implementations
* infinity variant of player.
*/
template <typename T>
InfinityPlayer<T>::InfinityPlayer(std::string name, T symbol, PlayerType type)
    : Player<T>(name, symbol, type) {}

/**
* @brief InfinityUI implementations
* infinity variant of user interface.
*/

template <typename T>
InfinityUI<T>::InfinityUI()
    : UI<T>("Welcome to Infinity Tic-Tac-Toe!\nOldest move disappears after every 3 moves.\nFirst to align 3 marks wins.", 3) {}

template <typename T>
Move<T>* InfinityUI<T>::get_move(Player<T>* player) {
    int x = -1, y = -1;

    if (player->get_type() == PlayerType::HUMAN) {
        /**
        * @brief Human player: prompt for input until valid move is entered.
        */        
        while (true) {
            std::cout << player->get_name() << " (" << player->get_symbol() << ") enter move (row col) [0..2]: ";
            if (!(std::cin >> x >> y)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Try again.\n";
                continue;
            }
            if (x < 0 || x >= 3 || y < 0 || y >= 3) {
                std::cout << "Out of range. Use 0..2.\n";
                continue;
            }
            if (player->get_board_ptr()->get_cell(x, y) != static_cast<T>(' ')) {
                std::cout << "Cell occupied. Pick another.\n";
                continue;
            }
            break;
        }
    }
    else {
        /**
		* @brief Simple AI: choose a random empty cell.
        */
        auto mat = player->get_board_ptr()->get_board_matrix();
        std::vector<std::pair<int, int>> empties;
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                if (mat[r][c] == static_cast<T>(' '))
                    empties.emplace_back(r, c);

        if (empties.empty()) {
            /**
            * @note This should not happen as game over is checked before AI move.
            */            
            x = y = 0;
        }
        else {
            int idx = std::rand() % static_cast<int>(empties.size());
            x = empties[idx].first;
            y = empties[idx].second;
        }
        std::cout << player->get_name() << " (" << player->get_symbol() << ") plays: " << x << " " << y << "\n";
    }

    return new Move<T>(x, y, player->get_symbol());
}

template <typename T>
Player<T>* InfinityUI<T>::create_player(std::string& name, T symbol, PlayerType type) {
    return new InfinityPlayer<T>(name, symbol, type);
}

/**
 * @brief Explicit instantiation for char type.
 */
template InfinityBoard<char>::InfinityBoard();
template bool InfinityBoard<char>::update_board(Move<char>*);
template bool InfinityBoard<char>::is_win(Player<char>*);
template bool InfinityBoard<char>::is_draw(Player<char>*);
template bool InfinityBoard<char>::is_lose(Player<char>*);
template bool InfinityBoard<char>::game_is_over(Player<char>*);

template class InfinityPlayer<char>;
template class InfinityUI<char>;
