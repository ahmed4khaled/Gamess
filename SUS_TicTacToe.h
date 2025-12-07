#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <utility>
#include <array>

enum class SUS_AIType { RANDOM = 1, SMART = 2 };

class SUS_Player : public Player<char> {
    SUS_AIType ai_type;
public:
    /**
	* @brief Construct a SUS player with name, symbol, type, and AI type.
    */
    SUS_Player(const std::string& name, char symbol, PlayerType type, SUS_AIType ai = SUS_AIType::SMART)
        : Player<char>(name, symbol, type), ai_type(ai) {}
    SUS_AIType get_ai_type() const { return ai_type; }
};

class SUS_Board : public Board<char> {
private:
    /**
	* @brief Scores for players S and U
    */
    int score_S;
    int score_U;

    bool in_bounds(int x, int y) const;

public:
    SUS_Board();

    /**
	* @brief Core overrides
    */
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

	/*
    *@brief Getters for main
    */
    int get_score_S() const { return score_S; }
    int get_score_U() const { return score_U; }

    /**
	* @brief Helper to let UI check occupancy
    * keeps encapsulation
    */
    char cell_at(int x, int y) const;

    /**
	* @brief AI tools
    */
    std::pair<int, int> random_move() const;
    std::pair<int, int> smart_move(char symbol) const;
};

class SUS_UI : public UI<char> {
public:
    SUS_UI() : UI<char>("S-U-S Game (3x3)", 3) {}

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    Player<char>** setup_players() override;

    /**
	* @brief get_move supports Human input validation (checks occupancy) and Computer moves
	* @return Move<char>* The move made by the player
    */
    Move<char>* get_move(Player<char>* player) override;
};

#endif 
