// LC 130 Surrounded Regions: in a board of 'X' and 'O', flip every 'O' region that is
// not connected (4-directionally) to the border into 'X', in place.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;

// Multi-source BFS from border 'O's marks the safe ones 'S'; then flip the rest.
// T: O(R*C), S: O(R*C).
void solve(vector<vector<char>>& board) {
    int m = board.size(), n = board[0].size();
    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    queue<pair<int, int>> q;   // safe cells whose neighbors are not checked yet
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            bool border = r == 0 || r == m - 1 || c == 0 || c == n - 1;
            if (border && board[r][c] == 'O') {
                board[r][c] = 'S';
                q.push({r, c});
            }
        }
    }
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (const auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            if (board[nr][nc] != 'O') { continue; }
            board[nr][nc] = 'S';
            q.push({nr, nc});
        }
    }
    for (auto& row : board) {
        for (char& cell : row) {
            if (cell == 'O') { cell = 'X'; }
            else if (cell == 'S') { cell = 'O'; }
        }
    }
}

void test(const string& label, vector<vector<char>> board,
          const vector<vector<char>>& want) {
    solve(board);
    cout << label << (board == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("flip", {{'X', 'X', 'X'}, {'X', 'O', 'X'}, {'X', 'X', 'X'}},
         {{'X', 'X', 'X'}, {'X', 'X', 'X'}, {'X', 'X', 'X'}});
    test("border-linked", {{'X', 'X', 'X'}, {'X', 'O', 'X'}, {'X', 'O', 'X'}},
         {{'X', 'X', 'X'}, {'X', 'O', 'X'}, {'X', 'O', 'X'}});
    test("diagonal-only", {{'O', 'X', 'X'}, {'X', 'O', 'X'}, {'X', 'X', 'X'}},
         {{'O', 'X', 'X'}, {'X', 'X', 'X'}, {'X', 'X', 'X'}});
}
