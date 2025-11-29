#ifndef ULTIMATETTT_H
#define ULTIMATETTT_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

class UltimateTTTBoard : public Board<char> {
private:
    // Winners for the 3x3 main grid (each cell corresponds to a 3x3 sub-board)
    std::vector<std::vector<char>> mainBoardWinners;

    // Next required small board (row,col) OR (-1,-1) = free to play anywhere open
    std::pair<int,int> nextRequired;

    // Helpers
    char checkSmallWinner(int br, int bc) const;
    bool isSmallClosed(int br, int bc) const;

public:
    UltimateTTTBoard();

    // required by UI
    bool is_cell_allowed(int full_r, int full_c) const;
    std::pair<int,int> get_next_required() const;

    // Base overrides
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override;
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;
    std::vector<std::vector<char>> get_board_matrix() const;

    // FIX: Declare checkMainWin here
    bool checkMainWin(char s);
};

class UltimateTTT_UI : public UI<char> {
public:
    UltimateTTT_UI();
    Move<char>* get_move(Player<char>* p) override;
};

#endif // ULTIMATETTT_H
