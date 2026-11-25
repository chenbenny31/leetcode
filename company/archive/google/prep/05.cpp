// no fresh at all -> 0
// no rotten at all -> -1 if any fresh else 0
// fresh never reach -> -1
// allow in-place modify -> mark rot in-place

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

// BFS with rotten ones as multi-source, T: O(mn), S: O(mn)
int orangesRotting(vector<vector<int>> grid) {
    if (grid.empty() || grid[0].empty()) { return 0; }
    int m = grid.size();
    int n = grid[0].size();
    int dr[] = {0, 0, +1, -1};
    int dc[] = {+1, -1, 0, 0};
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
            auto [cr, cc] = q.front();
            q.pop();
            for (int d = 0; d < 4; d++) {
                int nr = cr + dr[d];
                int nc = cc + dc[d];
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                if (grid[nr][nc] != 1) { continue; }

                // (nr, nc) is fresh
                grid[nr][nc] = 2; // mark before enqueue
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
    test("spread", {{2, 1, 1}, {0, 1, 0}}, 2);
    test("two-sources", {{2, 1, 1, 1, 2}}, 2);
    test("unreachable", {{2, 0, 1}}, -1);
    test("no-fresh", {{0, 2}}, 0);
}
