// ByteDance (FastPrep, phone, NG e-commerce): an n x n board is given as n strings of
// 'X', 'O' or '.' (empty). gameOver(board) returns true when some player fills a whole
// row, column, the main diagonal or the anti-diagonal, or when the board is full with
// no winner; otherwise false.
// Follow-up asked in the round: O(n^2) first, then O(n) and O(1). A static board can't
// be checked in O(1); that needs the incremental interface below (LC 348, also asked
// as its own onsite question): moves [row, col, player] (player 1 or 2, alternating,
// distinct cells, no move after a win); after each move return the player if that
// move completes a row / column / diagonal, else 0.

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::abs;

// true when all cells (r0 + k*dr, c0 + k*dc), k = 0..n-1, hold the same player
bool sameLine(const vector<string>& board, int r0, int c0, int dr, int dc) {
    int n = board.size();
    char first = board[r0][c0];
    if (first == '.') {
        return false;
    }
    for (int k = 1; k < n; k++) {
        if (board[r0 + k * dr][c0 + k * dc] != first) {
            return false;
        }
    }
    return true;
}

// Check every row, every column and both diagonals, then whether any cell is empty.
// T: O(n^2), S: O(1).
bool gameOver(const vector<string>& board) {
    int n = board.size();
    for (int i = 0; i < n; i++) {
        if (sameLine(board, i, 0, 0, 1) || sameLine(board, 0, i, 1, 0)) {
            return true;
        }
    }
    if (sameLine(board, 0, 0, 1, 1) || sameLine(board, 0, n - 1, 1, -1)) {
        return true;
    }

    for (const string& row : board) {
        for (char cell : row) {
            if (cell == '.') {
                return false;
            }
        }
    }
    // full board, no winner: a draw also ends the game
    return true;
}

// LC 348: player 1 adds +1 and player 2 adds -1 to the counters of the row, column and
// diagonals the move touches; a counter reaching +-n means that player owns the line.
class TicTacToe {
    int n;
    // rows[r] / cols[c] = (#player-1 cells) - (#player-2 cells) on that line
    vector<int> rows;
    vector<int> cols;
    int diag = 0;
    int anti = 0;

public:
    explicit TicTacToe(int size) : n(size), rows(size, 0), cols(size, 0) {}

    // T: O(1)
    int move(int r, int c, int player) {
        int add = 1;
        if (player == 2) {
            add = -1;
        }
        rows[r] += add;
        cols[c] += add;
        if (r == c) {
            diag += add;
        }
        if (r + c == n - 1) {
            anti += add;
        }

        if (abs(rows[r]) == n || abs(cols[c]) == n || abs(diag) == n || abs(anti) == n) {
            return player;
        }
        return 0;
    }
};

vector<int> ticTacToeWinners(int n, const vector<vector<int>>& moves) {
    TicTacToe game(n);
    vector<int> result;
    for (const auto& m : moves) {
        result.push_back(game.move(m[0], m[1], m[2]));
    }
    return result;
}

void testOver(const string& label, const vector<string>& board, bool want) {
    cout << label << (gameOver(board) == want ? " PASS\n" : " FAIL\n");
}

void testMoves(const string& label, int n, const vector<vector<int>>& moves,
               const vector<int>& want) {
    cout << label << (ticTacToeWinners(n, moves) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    testOver("col-win", {"XO.", "XO.", "X.."}, true);
    testOver("anti-diag", {"..O", ".O.", "OX."}, true);
    testOver("ongoing", {"XO", ".."}, false);
    testOver("draw", {"XOX", "XOO", "OXX"}, true);
    testOver("one-empty", {"."}, false);
    testMoves("row-win", 2, {{0, 0, 1}, {1, 1, 2}, {0, 1, 1}}, {0, 0, 1});
    testMoves("col-win-p2", 3,
              {{0, 0, 1}, {0, 1, 2}, {2, 2, 1}, {1, 1, 2}, {1, 0, 1}, {2, 1, 2}},
              {0, 0, 0, 0, 0, 2});
    testMoves("anti-win", 3, {{0, 2, 1}, {0, 0, 2}, {1, 1, 1}, {0, 1, 2}, {2, 0, 1}},
              {0, 0, 0, 0, 1});
}
