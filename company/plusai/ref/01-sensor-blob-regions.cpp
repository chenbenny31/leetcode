// PlusAI (reported Jun 2025): Sensor Blob Regions (Medium). Pattern: BFS flood fill.
// A lidar occupancy grid has 1 = occupied cell, 0 = free. Return the size of every
// 4-connected region of 1s (diagonals do not connect), sorted descending.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::sort;
using std::greater;

// This is LC 200 Number of Islands that also counts cells: BFS from each unvisited 1,
// zeroing cells in place as visited (grid is a copy). T: O(m*n), S: O(m*n).
vector<int> regionSizes(vector<vector<int>> grid) {
    vector<int> sizes;
    if (grid.empty()) { return sizes; }
    int m = grid.size();
    int n = grid[0].size();
    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 0) { continue; }
            // cells of this region reached but not yet expanded
            queue<pair<int, int>> q;
            q.push({r, c});
            // zero on push, not on pop, so no cell is queued twice
            grid[r][c] = 0;
            int size = 0;
            while (!q.empty()) {
                auto [curR, curC] = q.front();
                q.pop();
                size++;
                for (int d = 0; d < 4; d++) {
                    int nxtR = curR + dr[d];
                    int nxtC = curC + dc[d];
                    if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
                    if (grid[nxtR][nxtC] == 0) { continue; }
                    grid[nxtR][nxtC] = 0;
                    q.push({nxtR, nxtC});
                }
            }
            sizes.push_back(size);
        }
    }
    sort(sizes.begin(), sizes.end(), greater<int>());
    return sizes;
}

void test(const string& label, const vector<vector<int>>& grid, const vector<int>& want) {
    cout << label << (regionSizes(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-blobs", {{1, 1, 0}, {1, 0, 0}, {0, 0, 1}}, {3, 1});
    test("u-shape", {{1, 0, 1}, {1, 0, 1}, {1, 1, 1}}, {7});
    test("diagonal-apart", {{1, 0}, {0, 1}}, {1, 1});
    test("empty", {}, {});
}

// follow-up (reported "optimal space"): rows stream in one at a time -> keep labels for
//   the previous row only. Label each 1 in the new row, union it with its left and up
//   neighbors (union-find with a size per root); a previous-row root that touches no
//   new-row cell is a finished region, so emit its size. Relabel each row to fresh ids,
//   so memory stays O(cols); after the last row emit every remaining root.
