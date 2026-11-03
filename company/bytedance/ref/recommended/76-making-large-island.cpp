// ByteDance (jobright, phone) LC 827 Making A Large Island: n x n grid of 0/1; change at
// most one 0 to 1. Return the size of the largest 4-directional island afterwards.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::max;
using std::find;

// BFS labels every island with an id and records its size. Then each 0 joins the
// distinct islands around it: 1 + their sizes. T: O(n^2), S: O(n^2).
int largestIsland(const vector<vector<int>>& grid) {
    int n = grid.size();
    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};
    // id[r][c] = island id of a land cell, -1 for water / not labeled yet
    vector<vector<int>> id(n, vector<int>(n, -1));
    // islandSize[k] = number of cells in island k
    vector<int> islandSize;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 0 || id[r][c] != -1) { continue; }
            int k = islandSize.size();
            int count = 0;
            queue<pair<int, int>> q;
            q.push({r, c});
            id[r][c] = k;
            while (!q.empty()) {
                auto [cr, cc] = q.front();
                q.pop();
                count++;
                for (int d = 0; d < 4; d++) {
                    int nr = cr + dr[d];
                    int nc = cc + dc[d];
                    if (nr < 0 || nr >= n || nc < 0 || nc >= n) { continue; }
                    if (grid[nr][nc] == 0 || id[nr][nc] != -1) { continue; }
                    id[nr][nc] = k;
                    q.push({nr, nc});
                }
            }
            islandSize.push_back(count);
        }
    }
    // no 0 to flip: the best is the largest existing island
    int best = 0;
    for (int s : islandSize) { best = max(best, s); }
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 1) { continue; }
            // ids already added for this cell (at most 4)
            vector<int> used;
            int total = 1;
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) { continue; }
                int k = id[nr][nc];
                if (k == -1) { continue; }
                if (find(used.begin(), used.end(), k) != used.end()) { continue; }
                used.push_back(k);
                total += islandSize[k];
            }
            best = max(best, total);
        }
    }
    return best;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (largestIsland(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("bridge-two", {{1, 0, 1}, {0, 0, 0}, {0, 0, 0}}, 3);
    test("same-island-once", {{1, 1}, {1, 0}}, 4);
    test("no-water", {{1, 1}, {1, 1}}, 4);
    test("all-water", {{0, 0}, {0, 0}}, 1);
}
