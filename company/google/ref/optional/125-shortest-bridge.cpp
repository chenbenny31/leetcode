// 125 · Shortest Bridge — LC 934
// n x n grid of 0 water / 1 land w/ exactly two islands (4-dir)
// return min num of 0s to flip to connect the two islands
//
// clarify:
// num islands -> exactly two
// diagonal cells -> not connected
// modify grid? -> yes (taken by value here)
// island size -> DFS recursion depth; explicit stack if huge

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

int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// mark the island containing (r, c) as 2 and push its cells into q
void dfs(vector<vector<int>>& grid, int r, int c, queue<pair<int, int>>& q) {
    int n = grid.size();
    if (r < 0 || r >= n || c < 0 || c >= n || grid[r][c] != 1) { return; }
    grid[r][c] = 2;
    q.push({r, c});
    for (auto& d : dirs) { dfs(grid, r + d[0], c + d[1], q); }
}

// core idea: DFS first island (mark 2, queue its cells), then multi-source BFS by level
// level at which a 1 is first touched = answer
// T: O(n^2), S: O(n^2)
int shortestBridge(vector<vector<int>> grid) {
    int n = grid.size();
    // frontier of the BFS, starting with every cell of the first island
    queue<pair<int, int>> q;
    bool found = false;
    for (int r = 0; r < n && !found; r++) {
        for (int c = 0; c < n && !found; c++) {
            if (grid[r][c] == 1) {
                dfs(grid, r, c, q);
                found = true;
            }
        }
    }
    int level = 0;
    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            auto [curR, curC] = q.front();
            q.pop();
            for (auto& d : dirs) {
                int nxtR = curR + d[0];
                int nxtC = curC + d[1];
                if (nxtR < 0 || nxtR >= n || nxtC < 0 || nxtC >= n) { continue; }
                if (grid[nxtR][nxtC] == 1) { return level; }
                if (grid[nxtR][nxtC] == 0) {
                    grid[nxtR][nxtC] = 2;
                    q.push({nxtR, nxtC});
                }
            }
        }
        level++;
    }
    return -1;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (shortestBridge(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-by-two", {{0, 1}, {1, 0}}, 1);
    test("gap-two", {{0, 1, 0}, {0, 0, 0}, {0, 0, 1}}, 2);
    test("ring", {{1, 1, 1, 1, 1},
                  {1, 0, 0, 0, 1},
                  {1, 0, 1, 0, 1},
                  {1, 0, 0, 0, 1},
                  {1, 1, 1, 1, 1}}, 1);
    test("diagonal", {{1, 0, 0}, {0, 1, 0}, {0, 0, 0}}, 1);
}

// follow-up:
// weighted cells (build cost per cell) -> Dijkstra from the first island
// > 2 islands, connect all -> label islands, BFS pairwise bridge len, MST (Kruskal)
// very large island -> explicit stack instead of recursive DFS
