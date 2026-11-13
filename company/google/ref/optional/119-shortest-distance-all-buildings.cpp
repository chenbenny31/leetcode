// 119 · Shortest Distance from All Buildings — LC 317
// grid: 0 empty, 1 building, 2 obstacle; 4-dir moves through empty land only
// pick empty cell w/ min sum of shortest paths to every building; return sum or -1
//
// clarify:
// house -> on empty land only
// paths -> cannot cross buildings
// cell that misses one building -> invalid
// modify grid? -> yes (taken by value here)

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

// core idea: BFS from each building, sum into dist; walk only cells w/ grid == walk
// those were reached by all earlier buildings; lower them by 1; best reset per BFS
// T: O(B * m * n), S: O(m * n) — B = num buildings
int shortestDistance(vector<vector<int>> grid) {
    int m = grid.size();
    int n = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // dist[r][c] = sum of BFS distances from the buildings walked so far
    vector<vector<int>> dist(m, vector<int>(n, 0));
    // cells reached by every building so far hold this value
    int walk = 0;
    int best = -1;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] != 1) { continue; }
            best = -1;
            // cells of the current BFS level
            queue<pair<int, int>> q;
            q.push({r, c});
            int level = 0;
            while (!q.empty()) {
                level++;
                int size = q.size();
                for (int i = 0; i < size; i++) {
                    auto [curR, curC] = q.front();
                    q.pop();
                    for (auto& d : dirs) {
                        int nxtR = curR + d[0];
                        int nxtC = curC + d[1];
                        if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
                        if (grid[nxtR][nxtC] != walk) { continue; }
                        // marks the cell as reached by this building too
                        grid[nxtR][nxtC]--;
                        dist[nxtR][nxtC] += level;
                        q.push({nxtR, nxtC});
                        if (best == -1 || dist[nxtR][nxtC] < best) {
                            best = dist[nxtR][nxtC];
                        }
                    }
                }
            }
            walk--;
        }
    }
    return best;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (shortestDistance(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-building", {{1, 0, 0}}, 1);
    test("both-ends", {{1, 0, 0, 1}}, 3);
    test("walled-off", {{1, 2, 0}, {2, 0, 0}, {0, 0, 1}}, -1);
    test("no-empty", {{1, 2, 1}}, -1);
}

// follow-up:
// many buildings, few empty cells -> BFS from each empty cell instead (smaller side)
// Manhattan distance, no obstacles (LC 296) -> median of rows and of cols, O(m * n)
