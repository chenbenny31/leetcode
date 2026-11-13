// LC 200 Number of Islands (Medium) — grid flood fill (BFS) / union-find
// grid of '1' (land) and '0' (water); count the islands, where an island is land
// connected horizontally or vertically.
// Clarify: cells are chars, not ints; 4 directions only; empty grid; may I modify the
//   input; grid size (depth of a recursive DFS).

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

// Core idea: every land cell not visited yet starts a new island; a BFS from it sinks the
// whole island to '0', marking on push so no cell is queued twice.
// T: O(m*n), S: O(m*n).
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

// Core idea: each land cell starts as its own island; union it with its land neighbors
// below and to the right, and every union of two different roots removes one island.
// T: O(m*n log(m*n)), near O(m*n) with union by size, S: O(m*n).
int numIslandsUnionFind(const vector<vector<char>>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    // parent[id] = parent of cell id = r * n + c in the union-find
    vector<int> parent(m * n);
    for (int id = 0; id < m * n; id++) { parent[id] = id; }
    int dirs[2][2] = {{1, 0}, {0, 1}};
    int islands = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] != '1') { continue; }
            islands++;
            for (int d = 0; d < 2; d++) {
                int nxtR = r + dirs[d][0];
                int nxtC = c + dirs[d][1];
                if (nxtR >= m || nxtC >= n) { continue; }
                if (grid[nxtR][nxtC] != '1') { continue; }
                int rootCur = findRoot(parent, r * n + c);
                int rootNxt = findRoot(parent, nxtR * n + nxtC);
                // the two cells can already be one island
                if (rootCur == rootNxt) { continue; }
                parent[rootNxt] = rootCur;
                islands--;
            }
        }
    }
    return islands;
}

void test(const string& label, const vector<vector<char>>& grid, int want) {
    bool ok = numIslands(grid) == want && numIslandsUnionFind(grid) == want;
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
    return 0;
}

// follow-up (ideas only):
// - land added one by one, count after each add (LC 305, Google) -> union-find with
//   parent -1 = water: count++ per new land, count-- per union; a repeat changes nothing
// - max area of island (LC 695) -> the flood fill returns its cell count (counted on
//   push); keep the largest
// - regions enclosed by walls (LC 1254, Google) -> first flood every open region that
//   touches the border, then count the regions left
// - battleships (LC 419) -> count only top-left cells: an 'X' with no 'X' above and none
//   to the left; O(1) space, no fill
// - diagonal connectivity (Google) -> 8 entries in dirs
// - recursive DFS instead of BFS -> shorter, but up to m*n calls deep (9e4 on 300 x 300)
// - distinct island shapes (LC 694) -> record each island's cells relative to its first
//   cell and put the shape in a set
// - surrounded regions (LC 130) -> flood from the border 'O's and mark them safe; flip
//   every other 'O' to 'X'
// - Pacific Atlantic (LC 417) -> flood uphill (to cells >= current) from each ocean's
//   border; answer = the cells reached by both floods
