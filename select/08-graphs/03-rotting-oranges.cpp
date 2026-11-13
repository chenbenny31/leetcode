// LC 994 Rotting Oranges (Medium) — multi-source BFS by layers
// grid: 0 empty, 1 fresh, 2 rotten; each minute fresh 4-adjacent to rotten rots
// return minutes until no fresh left, or -1 if impossible
//
// clarify:
// no fresh at start -> 0
// fresh that never rots -> -1
// directions -> 4
// modify grid? -> yes, works on a by-value copy

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

// core idea: push all rotten first, BFS one layer per minute; fresh > 0 at end -> -1
// T: O(m * n), S: O(m * n)
int orangesRotting(vector<vector<int>> grid) {
    int m = grid.size();
    int n = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // oranges that rotted in the latest minute
    queue<pair<int, int>> q;
    int fresh = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 2) { q.push({r, c}); }
            if (grid[r][c] == 1) { fresh++; }
        }
    }
    int minutes = 0;
    // stop once fresh hits 0, so the last layer adds no extra minute
    while (!q.empty() && fresh > 0) {
        minutes++;
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++) {
            auto [curR, curC] = q.front();
            q.pop();
            for (int d = 0; d < 4; d++) {
                int nxtR = curR + dirs[d][0];
                int nxtC = curC + dirs[d][1];
                if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
                if (grid[nxtR][nxtC] != 1) { continue; }
                grid[nxtR][nxtC] = 2;
                fresh--;
                q.push({nxtR, nxtC});
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
    return 0;
}

// follow-up:
// dist to nearest 0 (LC 542, Google "nearest taxi") -> same BFS, no layers; dist on push
// walls and gates (LC 286) -> BFS from all gates in place; step only into INT_MAX rooms
// shortest clear path, 8 dirs (LC 1091) -> single-source BFS; check start / end first
// per-cell rot delays -> multi-source Dijkstra on time; ans = max time over fresh cells
// as far from land as possible (LC 1162) -> BFS from all land; last layer's dist = ans
