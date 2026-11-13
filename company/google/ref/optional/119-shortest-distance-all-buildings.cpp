// LC 317 Shortest Distance from All Buildings: grid of 0 empty land, 1 building,
// 2 obstacle; 4-directional moves through empty land only. Build a house on an empty
// cell so the sum of shortest path lengths to every building is minimal. Return that
// sum, or -1 if no empty cell reaches all buildings.
//
// Clarify:
// - house on empty land only; paths pass through empty land only
// - a cell that can't reach every building is skipped; no valid cell -> -1
// Idea: BFS from every building; dist[r][c] sums each cell's BFS distances. Pruning:
//   the k-th BFS walks only cells whose grid value is -k and sets them to -(k+1), so
//   a cell missed by one building is never visited again (no seen array, no reach
//   count). best is reset per BFS, so after the last BFS it only covers valid cells.
// Pitfalls:
// - reset best before every BFS, else a cell missed by a later building survives
// - BFS level by level so level = distance from the current building
// - a building walled off: later BFSes find no cell, best stays -1
// Tests to write: one building in a row; buildings at both ends; walled-off building;
//   no empty cell.

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

// BFS from each building over cells reached by all earlier buildings.
// T: O(B * m * n) for B buildings, S: O(m * n).
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

// follow-up: many buildings, few empty cells -> BFS from each empty cell instead
//   (BFS from the smaller side)
// follow-up: Manhattan distance, no obstacles (LC 296 best meeting point) -> median of
//   the rows and median of the columns separately, O(m * n)
