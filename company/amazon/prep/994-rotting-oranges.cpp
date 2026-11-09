#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;

int orangesRotting(vector<vector<int>> grid) {
    int m = grid.size();
    int n = grid.size();
    int dr[] = {1, -1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    queue<pair<int, int>> q;
    int fresh = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 2) { q.push({r, c}); }
            if (grid[r][c] == 1) { fresh++; }
        }
    }

    int minutes = 0;
    while (!q.empty() && fresh > 0) {
        minutes++;
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            auto [r, c] = q.front();
            q.pop();

            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                if (grid[nr][nc] != 1) { continue; }
                grid[nr][nc] = 2;
                fresh--;
                q.push({nr, nc});
            }
        }
    }
    return fresh == 0 ? minutes : -1;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (orangesRotting(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {{2, 1, 1}, {1, 1, 0}, {0, 1, 1}}, 4);
    test("unreachable", {{2, 1, 1}, {0, 1, 1}, {1, 0, 1}}, -1);
    test("no-fresh", {{0, 2}}, 0);
}
