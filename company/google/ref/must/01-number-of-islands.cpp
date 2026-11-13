// 01 · Number of Islands — LC 200, 305, 1254, 695, 419
// Mother problem: select/08-graphs/01-number-of-islands.cpp
// Google reports: LC 200 ×3, land added one by one ×3, enclosed regions ×3, max area ×2,
// battleships, diagonal ×2, connected crop garden (59) ×3
//
// LC 200: grid of '1' land / '0' water; count islands (land joined in 4 directions).
// Clarify
// - connectivity → 4 directions; grid at least 1 x 1; the function takes a copy
// - up to ~300 x 300 → a recursive DFS can go 9e4 deep, so use BFS
// Core idea
// - Each unvisited land cell starts an island; BFS from it sinks the island to '0'.
// - Mark on push, not on pop, so no cell is queued twice; bounds check before reading.
// - T O(m*n), S O(m*n).
// Tests: two islands → 2; U-shape → 1; diagonal touch → 2; all water → 0.
//
// Follow-ups
// - land added one by one (LC 305) → `numIslandsAfterAdds` below
// - max area (LC 695) → same BFS, return the count of each flood and keep the max
// - enclosed regions (LC 1254 style) → flood every border '0' region to '1' first, then
//   count the '0' regions left
// - battleships (LC 419) → count only 'X' cells with no 'X' above or to the left, O(1)
// - diagonal connectivity → 8 entries in `dirs`, loop d < 8
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

// root of cell id, halving the path on the way up
int findRoot(vector<int>& parent, int id) {
    while (parent[id] != id) {
        parent[id] = parent[parent[id]];
        id = parent[id];
    }
    return id;
}

// LC 305: an m x n grid of water; each position turns a cell into land; return the
// island count after each add (a BFS per add, O(k*m*n), is too slow).
// Union-find on id r * n + c: count++ per new land, count-- per union of two roots.
// Pitfalls: push the count on a repeated position too; find both roots inside the loop
// (an earlier union moves them). T: O(m*n + k log(m*n)), S: O(m*n); k = positions.
vector<int> numIslandsAfterAdds(int m, int n, const vector<vector<int>>& positions) {
    // parent[id] = parent cell of land id in the union-find, -1 = water
    vector<int> parent(m * n, -1);
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int count = 0;
    vector<int> result;
    for (const auto& pos : positions) {
        int id = pos[0] * n + pos[1];
        // already land: nothing changes
        if (parent[id] != -1) {
            result.push_back(count);
            continue;
        }
        parent[id] = id;
        count++;
        for (int d = 0; d < 4; d++) {
            int nxtR = pos[0] + dirs[d][0];
            int nxtC = pos[1] + dirs[d][1];
            if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
            int nxt = nxtR * n + nxtC;
            if (parent[nxt] == -1) { continue; }
            int rootCur = findRoot(parent, id);
            int rootNxt = findRoot(parent, nxt);
            // two neighbors can already be one island
            if (rootCur == rootNxt) { continue; }
            parent[rootNxt] = rootCur;
            count--;
        }
        result.push_back(count);
    }
    return result;
}

void test(const string& label, const vector<vector<char>>& grid, int want) {
    cout << label << (numIslands(grid) == want ? " PASS\n" : " FAIL\n");
}

void testAdds(const string& label, int m, int n, const vector<vector<int>>& positions,
              const vector<int>& want) {
    bool ok = numIslandsAfterAdds(m, n, positions) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
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
    testAdds("adds-bridge", 1, 3, {{0, 0}, {0, 2}, {0, 1}}, {1, 2, 1});
    testAdds("adds-diagonal-then-ring", 2, 2, {{0, 0}, {1, 1}, {0, 1}, {1, 0}},
             {1, 2, 1, 1});
    testAdds("adds-repeat", 1, 2, {{0, 1}, {0, 1}}, {1, 1});
}
