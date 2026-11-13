// LC 200 Number of Islands (Medium) — grid flood fill (BFS) / union-find
// grid of '1' land, '0' water; count islands (land connected up / down / left / right)
//
// clarify:
// cells -> chars, not ints
// directions -> 4 only; diagonal = 8 dirs
// empty grid -> assume not
// modify input? -> BFS sinks a by-value copy; else keep a visited grid
// grid size -> recursive DFS can go m * n deep, use BFS

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

// solution 1: BFS flood fill; default
// core idea: each unvisited land cell = new island; BFS from it sinks the island to '0'
// T: O(m * n), S: O(m * n)
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

// solution 2: union-find; use it when land is added online (LC 305)
// core idea: each land cell = own island; union w/ land below / right, each union = -1
// T: O(m * n log(m * n)), S: O(m * n) — path halving only, no union by size
int numIslandsUnionFind(const vector<vector<char>>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    // parent[id] = parent of cell id = r * n + c
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
                // already one island
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

// follow-up:
// land added one by one (LC 305, Google) -> union-find; cnt++ per new land, -- per union
// max area of island (LC 695) -> flood fill returns its cell cnt; keep max
// regions enclosed by walls (LC 1254, Google) -> flood regions on border, count the rest
// battleships (LC 419) -> count top-left cells only: no 'X' above or left; O(1) space
// diagonal connectivity (Google) -> 8 entries in dirs
// distinct island shapes (LC 694) -> cells relative to first cell, shape into a set
// surrounded regions (LC 130) -> flood from border 'O's, mark safe; flip other 'O' to 'X'
// Pacific Atlantic (LC 417) -> flood uphill from each ocean's border; ans = both reached
