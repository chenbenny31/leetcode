// LC 51 N-Queens (Hard). Pattern: row-by-row backtracking with column/diagonal flags.
// Return all ways to place n queens on an n x n board so that no two attack each
// other ('Q' = queen, '.' = empty).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// places a queen in row r, then recurses on row r + 1
void dfs(int r, int n, vector<string>& board, vector<bool>& cols, vector<bool>& diag,
         vector<bool>& anti, vector<vector<string>>& result) {
    if (r == n) {
        result.push_back(board);
        return;
    }
    for (int c = 0; c < n; c++) {
        // r + c is constant on a '/' diagonal, r - c on a '\' one (shifted by n - 1)
        int diagId = r + c;
        int antiId = r - c + n - 1;
        if (cols[c] || diag[diagId] || anti[antiId]) { continue; }
        cols[c] = true;
        diag[diagId] = true;
        anti[antiId] = true;
        board[r][c] = 'Q';
        dfs(r + 1, n, board, cols, diag, anti, result);
        board[r][c] = '.';
        cols[c] = false;
        diag[diagId] = false;
        anti[antiId] = false;
    }
}

// One queen per row; O(1) conflict checks via flags. T: O(n!), S: O(n^2).
vector<vector<string>> solveNQueens(int n) {
    // current partial placement
    vector<string> board(n, string(n, '.'));
    // cols[c] = column c taken
    vector<bool> cols(n, false);
    // diag[r + c] = '/' diagonal taken
    vector<bool> diag(2 * n - 1, false);
    // anti[r - c + n - 1] = '\' diagonal taken (shifted to be non-negative)
    vector<bool> anti(2 * n - 1, false);
    vector<vector<string>> result;
    dfs(0, n, board, cols, diag, anti, result);
    return result;
}

void test(const string& label, int n, int wantCount, const vector<string>& wantFirst) {
    vector<vector<string>> got = solveNQueens(n);
    bool ok = (int)got.size() == wantCount && (got.empty() || got[0] == wantFirst);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("none", 3, 0, {});
    test("five", 5, 10, {"Q....", "..Q..", "....Q", ".Q...", "...Q."});
    test("six", 6, 4, {".Q....", "...Q..", ".....Q", "Q.....", "..Q...", "....Q."});
}

// follow-up: count only (LC 52) -> same dfs without the board, return the sum
// follow-up: bitmask flags -> avail = ~(cols | d | a) & ((1 << n) - 1); take the
//   lowest bit p = avail & -avail, next row d = (d | p) << 1, a = (a | p) >> 1
