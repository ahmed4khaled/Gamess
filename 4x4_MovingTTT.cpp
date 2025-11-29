#include "4x4_MovingTTT.h"

MovingTTT4x4_Board::MovingTTT4x4_Board() : Board(4, 4) {

    // Fill board with spaces
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            board[i][j] = ' ';

    // X starting
    board[0][0] = 'X';
    board[0][1] = 'X';
    board[0][2] = 'X';
    board[0][3] = 'X';

    // O starting
    board[3][0] = 'O';
    board[3][1] = 'O';
    board[3][2] = 'O';
    board[3][3] = 'O';
}


bool MovingTTT4x4_Board::in_bounds(int r, int c) {
    return (r >= 0 && r < 4 && c >= 0 && c < 4);
}

bool MovingTTT4x4_Board::update_board(Move<char>* move) {
    char sym = move->get_symbol();
    int r = move->get_x();
    int c = move->get_y();

    // Format of move: we encode from and to in one move:
    // x = from_row*10 + to_row
    // y = from_col*10 + to_col

    int fr = r / 10;
    int tr = r % 10;
    int fc = c / 10;
    int tc = c % 10;

    if (!in_bounds(fr, fc) || !in_bounds(tr, tc))
        return false;

    if (board[fr][fc] != sym) return false;
    if (board[tr][tc] != ' ') return false;

    // Must move 1 step horizontally or vertically
    if (abs(fr - tr) + abs(fc - tc) != 1)
        return false;

    // Apply movement
    board[fr][fc] = ' ';
    board[tr][tc] = sym;

    return true;
}

bool MovingTTT4x4_Board::check_three_in_row(char sym) {
    // rows
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 2; c++)
            if (board[r][c] == sym && board[r][c+1] == sym && board[r][c+2] == sym)
                return true;

    // columns
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 2; r++)
            if (board[r][c] == sym && board[r+1][c] == sym && board[r+2][c] == sym)
                return true;

    // diagonals
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 2; c++)
            if (board[r][c] == sym && board[r+1][c+1] == sym && board[r+2][c+2] == sym)
                return true;

    for (int r = 0; r < 2; r++)
        for (int c = 2; c < 4; c++)
            if (board[r][c] == sym && board[r+1][c-1] == sym && board[r+2][c-2] == sym)
                return true;

    return false;
}

bool MovingTTT4x4_Board::is_win(Player<char>* p) {
    return check_three_in_row(p->get_symbol());
}

bool MovingTTT4x4_Board::is_draw(Player<char>* p) {
    return false; // No draw in a movement game
}

//////////////////////////////////////////////////////
// UI
//////////////////////////////////////////////////////

Move<char>* MovingTTT4x4_UI::get_move(Player<char>* player) {

    // ====== COMPUTER MOVE ======
    if (player->get_type() == PlayerType::COMPUTER) {

        auto* board = player->get_board_ptr();
        auto mat = board->get_board_matrix();

        vector<pair<pair<int,int>, pair<int,int>>> moves;

        // Scan all pieces of computer
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {

                if (mat[r][c] == player->get_symbol()) {

                    int dr[4] = {-1,+1,0,0};
                    int dc[4] = {0,0,-1,+1};

                    for (int i = 0; i < 4; i++) {
                        int nr = r + dr[i];
                        int nc = c + dc[i];

                        if (nr >= 0 && nr < 4 &&
                            nc >= 0 && nc < 4 &&
                            mat[nr][nc] == ' ')
                        {
                            moves.push_back({{r,c},{nr,nc}});
                        }
                    }
                }
            }
        }

        if (moves.empty()) {
            cout << "Computer has no moves.\n";
            return new Move<char>(0,0,player->get_symbol());
        }

        int k = rand() % moves.size();
        int fr = moves[k].first.first;
        int fc = moves[k].first.second;
        int tr = moves[k].second.first;
        int tc = moves[k].second.second;

        cout << "Computer moves: (" << fr << "," << fc << ") → (" << tr << "," << tc << ")\n";

        int x = fr * 10 + tr;
        int y = fc * 10 + tc;
        return new Move<char>(x, y, player->get_symbol());
    }

    // ====== HUMAN MOVE ======
    int fr, fc, tr, tc;
    cout << player->get_name() << " (" << player->get_symbol() << ") move:\n";
    cout << "FROM row col: ";
    cin >> fr >> fc;
    cout << "TO row col: ";
    cin >> tr >> tc;

    int x = fr * 10 + tr;
    int y = fc * 10 + tc;

    return new Move<char>(x, y, player->get_symbol());
}
