// ByteDance (hack2hire) / LC 200: grid of '1' (land) and '0' (water); count islands,
// where an island is land connected horizontally or vertically.
// Follow-up LC 694: count distinct island shapes (same up to translation only).
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::set;

// BFS flood fill per new island, sinking land to '0'. T: O(m*n), S: O(m*n).
int numIslands(vector<vector<char>> grid) {
    int m = grid.size();
    int n = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int islands = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] != '1') { continue; }
            islands++;
            // land cells of this island still to expand
            queue<pair<int, int>> q;
            q.push({r, c});
            // mark on push, so no cell is queued twice
            grid[r][c] = '0';
            while (!q.empty()) {
                auto [curR, curC] = q.front();
                q.pop();
                for (int d = 0; d < 4; d++) {
                    int nr = curR + dirs[d][0];
                    int nc = curC + dirs[d][1];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                    if (grid[nr][nc] != '1') { continue; }
                    grid[nr][nc] = '0';
                    q.push({nr, nc});
                }
            }
        }
    }
    return islands;
}

// follow-up: same BFS; the shape key is each cell's offset from the island's first
// cell, in visit order. T: O(m*n log I), S: O(m*n); I = islands.
int numDistinctIslands(vector<vector<char>> grid) {
    int m = grid.size();
    int n = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // distinct offset lists seen so far
    set<vector<pair<int, int>>> shapes;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] != '1') { continue; }
            // same shape + same scan-first cell + fixed dir order -> same visit order
            // shape = (row - r, col - c) per cell
            vector<pair<int, int>> shape;
            queue<pair<int, int>> q;
            q.push({r, c});
            grid[r][c] = '0';
            while (!q.empty()) {
                auto [curR, curC] = q.front();
                q.pop();
                shape.push_back({curR - r, curC - c});
                for (int d = 0; d < 4; d++) {
                    int nr = curR + dirs[d][0];
                    int nc = curC + dirs[d][1];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                    if (grid[nr][nc] != '1') { continue; }
                    grid[nr][nc] = '0';
                    q.push({nr, nc});
                }
            }
            shapes.insert(shape);
        }
    }
    return shapes.size();
}

void test(const string& label, const vector<vector<char>>& grid, int want) {
    cout << label << (numIslands(grid) == want ? " PASS\n" : " FAIL\n");
}

void testDistinct(const string& label, const vector<vector<char>>& grid, int want) {
    cout << label << (numDistinctIslands(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three", {{'1', '1', '0', '0', '0'},
                   {'1', '1', '0', '0', '0'},
                   {'0', '0', '1', '0', '0'},
                   {'0', '0', '0', '1', '1'}}, 3);
    test("diagonal", {{'1', '0'}, {'0', '1'}}, 2);
    test("all-water", {{'0'}}, 0);
    testDistinct("two-shapes", {{'1', '1', '0', '1', '1'},
                                {'0', '0', '0', '0', '0'},
                                {'1', '0', '0', '0', '1'}}, 2);
}

// follow-up: distinct shapes (LC 694) -> see numDistinctIslands() above; offsets from the
//   first cell into set<vector<pair<int, int>>>
// follow-up: land added one cell at a time (LC 305) -> union-find on id r * n + c;
//   count++ per add, count-- per successful union; O(m*n + k * alpha)
// follow-up: max area (LC 695) -> count cells popped per BFS, keep the max
