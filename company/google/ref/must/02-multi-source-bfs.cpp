// 02 · Rotting Oranges (Multi-source BFS) — LC 994, 286, 1091
// Mother: select/08-graphs/03-rotting-oranges.cpp
// LC 994: grid of 0 empty, 1 fresh, 2 rotten; each min fresh 4-adjacent to rotten rots
// return minutes until no fresh left, -1 if impossible
//
// clarify:
// no fresh at start -> 0
// no rotten at all -> -1 if any fresh, else 0
// fresh never reached -> -1
// may modify the grid -> yes (takes a copy), mark rot in place

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

// core idea: push all rotten first, BFS one layer per minute
// cnt fresh up front, fresh-- on each rot; fresh > 0 at the end -> -1
// T: O(m * n), S: O(m * n)
int orangesRotting(vector<vector<int>> grid) {
    int m = grid.size();
    int n = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // q = oranges that rotted in the latest minute
    queue<pair<int, int>> q;
    int fresh = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 2) { q.push({r, c}); }
            if (grid[r][c] == 1) { fresh++; }
        }
    }
    int minutes = 0;
    // stop once fresh = 0 -> last layer does not cnt an extra minute
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

// follow-up:
// dist to nearest 0 (LC 542) -> multi-source BFS from every 0; dist[nxt] = dist[cur] + 1
// walls and gates (LC 286) -> multi-source BFS from all gates, step only into INT_MAX
// shortest clear path, 8 dirs (LC 1091) -> plain BFS from (0, 0); blocked ends -> -1
// per-cell rot delays -> multi-source Dijkstra on time; answer = max time over fresh
// return a book via trains -> multi-source BFS from stations (Dijkstra if costs differ)
// broken-node path / server BFS series -> 03
// one wall break -> 06; safest path -> 35
