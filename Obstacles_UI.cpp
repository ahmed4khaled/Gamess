#include "Obstacles_UI.h"
#include "Obstacles_Board.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>

/* ============================================================
 *  CONSTRUCTOR
 * ============================================================ */

Obstacles_UI::Obstacles_UI() : UI<char>(2) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

/* ============================================================
 *  DISPLAY
 * ============================================================ */

void Obstacles_UI::display_board_matrix(const std::vector<std::vector<char>>& m) const {
    std::cout << "\n     0   1   2   3   4\n";
    std::cout <<   "   ---------------------\n";
    for (int r = 0; r < 5; r++) {
        std::cout << r << " |";
        for (int c = 0; c < 5; c++)
            std::cout << " " << m[r][c] << " |";
        std::cout << "\n   ---------------------\n";
    }
}

/* ============================================================
 *  SAFE INPUT
 * ============================================================ */

int Obstacles_UI::read_int_in_range(int minv, int maxv) const {
    int x;
    while (true) {
        if (std::cin >> x && x >= minv && x <= maxv)
            return x;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid. Enter [" << minv << " - " << maxv << "]: ";
    }
}

/* ============================================================
 *  CREATE PLAYER
 * ============================================================ */

Player<char>* Obstacles_UI::create_player(std::string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

/* ============================================================
 *  PREPARE OBSTACLES
 * ============================================================ */

void Obstacles_UI::prepare_obstacles(Board<char>* baseBoard) {
    auto* board = dynamic_cast<Obstacles_Board*>(baseBoard);
    if (!board) return;

    std::vector<std::pair<int,int>> obs;

    // Example: place 3 obstacles randomly
    int placed = 0;
    while (placed < 3) {
        int r = rand() % 5;
        int c = rand() % 5;
        if (board->get_cell(r,c) == '-') {
            obs.emplace_back(r,c);
            placed++;
        }
    }

    board->place_obstacles(obs);
}

/* ============================================================
 *  GET MOVE (HUMAN OR AI)
 * ============================================================ */

Move<char>* Obstacles_UI::get_move(Player<char>* p) {
    Board<char>* b = p->get_board_ptr();
    auto mat = b->get_board_matrix();
    char sym = p->get_symbol();

    // ---------- HUMAN ----------
    if (p->get_type() == PlayerType::HUMAN) {
        std::cout << p->get_name() << " (" << sym << ") choose row 0–4: ";
        int r = read_int_in_range(0,4);

        std::cout << "choose col 0–4: ";
        int c = read_int_in_range(0,4);

        return new Move<char>(r, c, sym);
    }

    // ---------- AI ----------
    // Collect empty cells
    std::vector<std::pair<int,int>> empties;
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            if (mat[r][c] == '-')
                empties.emplace_back(r,c);

    if (empties.empty())
        return new Move<char>(0,0,sym);

    int idx = rand() % empties.size();
    return new Move<char>(empties[idx].first, empties[idx].second, sym);
}
