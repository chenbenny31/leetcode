// LC 994 Rotting Oranges: grid cell 0 = empty, 1 = fresh, 2 = rotten. Each minute a
// fresh orange next to a rotten one (4-directional) rots. Return the minutes until
// none is fresh, or -1 if some never rot.
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

// Multi-source level BFS from all rotten oranges; one layer = one minute.
// T: O(R*C), S: O(R*C).
int orangesRotting(vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size();
    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    queue<pair<int, int>> q;   // oranges that rotted in the latest minute
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
        // freeze the layer size: oranges pushed now rot next minute
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            auto [r, c] = q.front();
            q.pop();
            for (const auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
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

void test(const string& label, vector<vector<int>> grid, int want) {
    cout << label << (orangesRotting(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {{2, 1, 1}}, 2);
    test("two-sources", {{2, 1, 2}}, 1);
    test("walled-off", {{2, 0, 1}}, -1);
    test("no-fresh", {{0, 2}}, 0);
}
