// 02 · Rotting Oranges (Multi-source BFS) — LC 994, 286, 1091
// Mother problem: select/08-graphs/03-rotting-oranges.cpp
// Google reports: broken-node path ×4 and server BFS series (03), hop path ×3, land path
// ×3, grid shortest path with one break (06), return a book ×2, safest path grid (35),
// count shortest paths / nodes on any shortest path / escape a pursuer (03)
//
// LC 994: grid of 0 empty, 1 fresh, 2 rotten; each minute fresh oranges 4-adjacent to a
// rotten one rot. Minutes until no fresh orange remains, -1 if impossible.
// Clarify
// - no fresh at the start → 0, even with no rotten one; a fresh one never reached → -1
// - the function takes a copy; ~1e4 cells → one BFS
// Core idea
// - Push every rotten orange before the loop; one BFS layer = one minute.
// - Count fresh first and decrement on each rot; fresh > 0 at the end → -1.
// - Save q.size() before a layer; loop while fresh > 0 so the last layer adds no minute.
// - Mark on push. T O(m*n), S O(m*n).
// Tests: spread around an empty cell → 2; two sources meet; walled off → -1; no fresh.
//
// Follow-ups
// - distance to the nearest 0 (LC 542) → same multi-source BFS from every 0, no layers:
//   dist[nxt] = dist[cur] + 1 set on push
// - walls and gates (LC 286) → multi-source BFS from all gates, step only into INT_MAX
//   rooms
// - shortest clear path, 8 directions (LC 1091) → plain BFS from (0, 0) with dist =
//   cells so far and 8 `dirs`; blocked start or end → -1
// - per-cell rot delays → multi-source Dijkstra on time: min-heap of (time, r, c), skip
//   stale pops, answer = max time over fresh cells (-1 if one is never reached)
//
//////////////////////////////////////////////////////////////////////////////////////////

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

// BFS from all rotten oranges at once, one layer per minute. T: O(m*n), S: O(m*n).
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
    // stop once fresh hits 0, so the last layer does not count an extra minute
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
}
