// LC 37 Sudoku Solver: fill the '.' cells of a 9x9 board with digits 1-9 so every row,
// column and 3x3 box holds each digit once. The puzzle has a solution; modify in place.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// rows[r], cols[c], boxes[b] are bitmasks: bit d set = digit d is already used there.
// Fill cells in reading order; on a dead end, undo the digit and try the next.
bool dfs(vector<vector<char>>& board, int pos, vector<int>& rows, vector<int>& cols,
         vector<int>& boxes) {
    if (pos == 81) { return true; }
    int r = pos / 9, c = pos % 9, b = r / 3 * 3 + c / 3;
    if (board[r][c] != '.') { return dfs(board, pos + 1, rows, cols, boxes); }
    for (int d = 1; d <= 9; d++) {
        int bit = 1 << d;
        if ((rows[r] | cols[c] | boxes[b]) & bit) { continue; }
        rows[r] |= bit;
        cols[c] |= bit;
        boxes[b] |= bit;
        board[r][c] = '0' + d;
        if (dfs(board, pos + 1, rows, cols, boxes)) { return true; }
        board[r][c] = '.';
        rows[r] ^= bit;
        cols[c] ^= bit;
        boxes[b] ^= bit;
    }
    return false;
}

// Backtracking over empty cells with constraint bitmasks. T: O(9^E) worst case for E
// empty cells, far less in practice; S: O(81) recursion.
void solveSudoku(vector<vector<char>>& board) {
    vector<int> rows(9, 0), cols(9, 0), boxes(9, 0);
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (board[r][c] == '.') { continue; }
            int bit = 1 << (board[r][c] - '0');
            rows[r] |= bit;
            cols[c] |= bit;
            boxes[r / 3 * 3 + c / 3] |= bit;
        }
    }
    dfs(board, 0, rows, cols, boxes);
}

// a valid completed grid
vector<vector<char>> filled() {
    vector<vector<char>> grid(9, vector<char>(9));
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            grid[r][c] = '1' + (r * 3 + r / 3 + c) % 9;
        }
    }
    return grid;
}

void test(const string& label, vector<vector<char>> board) {
    vector<vector<char>> want = filled();
    solveSudoku(board);
    cout << label << (board == want ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<vector<char>> diagonal = filled();
    vector<vector<char>> topRow = filled();
    vector<vector<char>> leftBoxes = filled();
    for (int i = 0; i < 9; i++) {
        diagonal[i][i] = '.';
        topRow[0][i] = '.';
    }
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 3; c++) {
            if ((r + c) % 2 == 0) { leftBoxes[r][c] = '.'; }
        }
    }
    test("already-full", filled());
    test("diagonal-blank", diagonal);
    test("top-row-blank", topRow);
    test("left-checker-blank", leftBoxes);
}
