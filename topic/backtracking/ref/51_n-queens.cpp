// LC 51 N-Queens: place n queens on an n x n board so none attack each other. Return
// every distinct board as rows of 'Q' and '.'.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

void dfs(int n, int row, vector<bool>& cols, vector<bool>& diag, vector<bool>& anti,
         vector<string>& board, vector<vector<string>>& out) {
    if (row == n) {
        out.push_back(board);
        return;
    }
    for (int c = 0; c < n; c++) {
        // cells on one diagonal share row - c, on one anti-diagonal share row + c
        if (cols[c] || diag[row - c + n - 1] || anti[row + c]) { continue; }
        cols[c] = diag[row - c + n - 1] = anti[row + c] = true;
        board[row][c] = 'Q';
        dfs(n, row + 1, cols, diag, anti, board, out);
        board[row][c] = '.';
        cols[c] = diag[row - c + n - 1] = anti[row + c] = false;
    }
}

// Backtracking one queen per row, with constraint tables for attacked columns and
// diagonals. T: O(n!), S: O(n).
vector<vector<string>> solveNQueens(int n) {
    vector<bool> cols(n, false);        // cols[c] = column c has a queen
    vector<bool> diag(2 * n, false);    // diag[row - c + n - 1] = that diagonal has one
    vector<bool> anti(2 * n, false);    // anti[row + c] = that anti-diagonal has one
    vector<string> board(n, string(n, '.'));
    vector<vector<string>> out;
    dfs(n, 0, cols, diag, anti, board, out);
    return out;
}

void test(const string& label, int n, vector<vector<string>> want) {
    vector<vector<string>> got = solveNQueens(n);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("four", 4, {{"..Q.", "Q...", "...Q", ".Q.."}, {".Q..", "...Q", "Q...", "..Q."}});
    test("one", 1, {{"Q"}});
    test("no-solution", 3, {});
}
