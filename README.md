# FCAI Board Games (C++)

A modular C++ console application that implements multiple board games under one reusable engine.  
This project demonstrates object-oriented design, template-based abstractions, and game-logic engineering across a family of Tic-Tac-Toe variants and grid games.

## Why This Project Is CV-Ready

- Built a reusable game framework (`Board`, `UI`, `Player`, `Move`, `GameManager`) using C++ templates and polymorphism.
- Implemented 14 playable games/variants in a single executable with a menu-driven launcher.
- Applied separation of concerns by isolating game rules, interaction logic, and game flow.
- Used structured documentation/comments and generated docs artifacts (`html/`).
- Demonstrates practical software engineering skills: OOP, maintainability, extensibility, and problem decomposition.

## Tech Stack

- Language: C++
- Paradigm: Object-Oriented Programming (inheritance, polymorphism, abstraction)
- Interface: Console-based UI
- Build: Any standard C++ compiler (g++, clang++, MSVC)

## Implemented Games

1. Normal XO  
2. Misere Tic-Tac-Toe  
3. Numerical Tic-Tac-Toe  
4. Diamond Tic-Tac-Toe  
5. 5x5 Tic-Tac-Toe  
6. Word Game  
7. Obstacles Game  
8. Ultimate Tic-Tac-Toe  
9. 4x4 Moving Tic-Tac-Toe  
10. Pyramid Tic-Tac-Toe  
11. Memory Tic-Tac-Toe  
12. SUS Tic-Tac-Toe  
13. Connect Four  
14. Infinity Tic-Tac-Toe

## Architecture Overview

- `BoardGame_Classes.h`: Core generic engine:
  - `Board<T>`: base board abstraction
  - `Move<T>`: move model
  - `Player<T>`: player model (human/computer)
  - `UI<T>`: interaction contract
  - `GameManager<T>`: turn loop and game lifecycle
- `main.cpp`: Central menu and game wiring
- `*.h` / `*.cpp` files per game: rule-specific board and UI implementations
- `dic.txt`: dictionary data used by the Word game

## How to Run

From the `Gamess` directory, compile and run with any C++17-compatible compiler.

### g++ (Linux/macOS/MinGW)

```bash
g++ -std=c++17 -O2 *.cpp -o board_games
./board_games
```

### MSVC (Developer Command Prompt)

```bat
cl /EHsc /std:c++17 *.cpp /Fe:board_games.exe
board_games.exe
```

##  Bullet Points

- Designed and developed a reusable C++ board-game engine using templates, inheritance, and polymorphism.
- Implemented 14 game variants (including Ultimate Tic-Tac-Toe and Connect Four) with clean separation between logic and UI.
- Improved extensibility by standardizing game flow through a generic `GameManager` and per-game board/UI modules.
- Produced maintainable, documented code suitable for collaborative academic/team environments.

## Future Enhancements

- Add unit tests for each game rule set.
- Add difficulty levels for AI players.
- Introduce a cross-platform GUI layer while reusing the same core engine.
- Add CI pipeline and automated build/test checks.

## Author

Developed as part of FCAI board games coursework/project work.

