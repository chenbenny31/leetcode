// LC 200 Number of Islands (Medium). Pattern: grid BFS flood fill.
// grid of '1' (land) and '0' (water); count islands, where an island is land connected
// horizontally or vertically.

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

// BFS from each unvisited land cell, sinking its island to '0'. T: O(m*n), S: O(m*n).
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
                    int nxtR = curR + dirs[d][0];
                    int nxtC = curC + dirs[d][1];
                    if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
                    if (grid[nxtR][nxtC] != '1') { continue; }
                    grid[nxtR][nxtC] = '0';
                    q.push({nxtR, nxtC});
                }
            }
        }
    }
    return islands;
}

void test(const string& label, const vector<vector<char>>& grid, int want) {
    cout << label << (numIslands(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-islands", {{'1', '1', '0'},
                         {'0', '0', '0'},
                         {'0', '1', '1'}}, 2);
    test("u-shape", {{'1', '0', '1'},
                     {'1', '0', '1'},
                     {'1', '1', '1'}}, 1);
    test("diagonal", {{'1', '0'}, {'0', '1'}}, 2);
    test("all-water", {{'0'}}, 0);
}

// follow-up: max area (LC 695) -> count cells popped per BFS, keep the max
// follow-up: land added one cell at a time (LC 305) -> union-find on id r * n + c;
//   count++ per add, count-- per successful union; O(m*n + k * alpha)
