// LC 36 Valid Sudoku (Medium). Pattern: seen flags per row, column, box.
// Is a partially filled 9x9 board valid? Each row, column and 3x3 box must hold digits
// 1-9 without repeats; '.' is empty. Solvability is not required.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// One pass; each digit marks its row, column and box. T: O(81), S: O(81).
bool isValidSudoku(const vector<vector<char>>& board) {
    // rows[r][digit] = digit already in row r (digit 0..8); same for cols and boxes
    vector<vector<bool>> rows(9, vector<bool>(9, false));
    vector<vector<bool>> cols(9, vector<bool>(9, false));
    vector<vector<bool>> boxes(9, vector<bool>(9, false));
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (board[r][c] == '.') {
                continue;
            }
            // '1'..'9' -> 0..8
            int digit = board[r][c] - '1';
            // box index 0..8, row-major
            int box = (r / 3) * 3 + c / 3;
            if (rows[r][digit] || cols[c][digit] || boxes[box][digit]) {
                return false;
            }
            rows[r][digit] = true;
            cols[c][digit] = true;
            boxes[box][digit] = true;
        }
    }
    return true;
}

// cells = {r, c, digit} placed on an otherwise empty board
void test(const string& label, const vector<vector<int>>& cells, bool want) {
    vector<vector<char>> board(9, vector<char>(9, '.'));
    for (const auto& cell : cells) {
        board[cell[0]][cell[1]] = '0' + cell[2];
    }
    cout << label << (isValidSudoku(board) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("ok", {{0, 0, 5}, {4, 4, 5}}, true);
    test("row", {{0, 0, 5}, {0, 8, 5}}, false);
    test("col", {{0, 0, 5}, {8, 0, 5}}, false);
    test("box", {{0, 0, 5}, {1, 1, 5}}, false);
}

// follow-up: less memory -> one int bitmask per row / column / box, bit d = digit seen
// follow-up: solve it (LC 37) -> backtracking over empty cells with the same three
//   tables: place a digit, recurse, unmark on the way back
