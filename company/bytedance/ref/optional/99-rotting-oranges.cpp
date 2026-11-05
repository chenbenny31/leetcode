// ByteDance / LC 994: grid of 0 (empty), 1 (fresh), 2 (rotten); each minute fresh
// oranges next to a rotten one (4-directionally) rot. Return the minutes until no fresh
// orange remains, or -1 if impossible.
#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;

// Multi-source BFS from all rotten oranges, one layer per minute. T: O(R*C), S: O(R*C).
int orangesRotting(vector<vector<int>> grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // oranges that rotted in the latest minute
    queue<pair<int, int>> q;
    int fresh = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == 2) { q.push({r, c}); }
            if (grid[r][c] == 1) { fresh++; }
        }
    }
    int minutes = 0;
    // stop once fresh hits 0, so the last layer does not count an extra minute
    while (!q.empty() && fresh > 0) {
        minutes++;
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            auto [curR, curC] = q.front();
            q.pop();
            for (int k = 0; k < 4; k++) {
                int nr = curR + dirs[k][0];
                int nc = curC + dirs[k][1];
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) { continue; }
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

// follow-up: no layer loop -> store the minute in grid: grid[nxt] = grid[cur] + 1;
//   answer = max(0, max grid value - 2), still -1 if any 1 remains
// follow-up: nearest-source distance (LC 542 / LC 286) -> same multi-source BFS,
//   dist set on push, O(R*C)
// follow-up: per-cell rot delays (weighted) -> multi-source Dijkstra, min-heap on time,
//   O(RC log RC)
