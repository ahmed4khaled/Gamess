#pragma once
#include "BoardGame_Classes.h"
#include <string>
#include <vector>

/**
 * @class Obstacles_UI
 * @brief UI handler for the 5×5 Obstacles Tic-Tac-Toe game.
 *
 * Responsibilities:
 * - Display the board
 * - Ask for human moves
 * - Provide simple AI:
 *     • Try to win
 *     • Try to block
 *     • Otherwise random empty move
 * - Insert obstacles before game starts
 */
class Obstacles_UI : public UI<char> {
public:
    Obstacles_UI();
    virtual ~Obstacles_UI() = default;

    /**
     * @brief Requests user or AI move.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Creates human or computer player.
     */
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /**
     * @brief Shows the 5×5 grid.
     */
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;

    /**
     * @brief Called once at game start: inserts obstacles.
     */
    void prepare_obstacles(Board<char>* board);

private:
    /**
     * @brief Safely reads an integer between min and max.
     */
    int read_int_in_range(int minv, int maxv) const;
};
