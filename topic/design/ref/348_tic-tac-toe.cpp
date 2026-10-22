// LC 348 Design Tic-Tac-Toe: n x n board, two players. move(row, col, player) places
// the player's mark and returns the player who wins with it (n in a row, column or
// diagonal), else 0. Every move is valid.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

class TicTacToe {
    int n;
    vector<int> rows;   // rows[r] = player 1 marks minus player 2 marks in row r
    vector<int> cols;   // same for each column
    int diag = 0;       // same for the main diagonal
    int anti = 0;       // same for the anti-diagonal

public:
    TicTacToe(int n) : n(n), rows(n, 0), cols(n, 0) {}

    // a line is won when its signed count reaches +n or -n. T: O(1), S: O(n).
    int move(int row, int col, int player) {
        int d = player == 1 ? 1 : -1;
        rows[row] += d;
        cols[col] += d;
        if (row == col) { diag += d; }
        if (row + col == n - 1) { anti += d; }
        if (rows[row] == n * d || cols[col] == n * d) { return player; }
        if (diag == n * d || anti == n * d) { return player; }
        return 0;
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    TicTacToe t(3);
    t.move(0, 0, 1);
    t.move(1, 1, 2);
    t.move(0, 1, 1);
    check(t.move(2, 2, 2) == 0, "no-winner");
    check(t.move(0, 2, 1) == 1, "row");
    TicTacToe u(3);
    u.move(0, 0, 2);
    u.move(0, 1, 1);
    u.move(1, 1, 2);
    u.move(1, 0, 1);
    check(u.move(2, 2, 2) == 2, "diagonal");
    TicTacToe v(3);
    v.move(0, 2, 1);
    v.move(0, 0, 2);
    v.move(1, 1, 1);
    v.move(1, 0, 2);
    check(v.move(2, 0, 1) == 1, "anti-diagonal");
}
