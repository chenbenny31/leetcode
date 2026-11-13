// 01 · Number of Islands — LC 200, 305, 1254, 695, 419
// Mother: select/08-graphs/01-number-of-islands.cpp
// (1) LC 200: grid of '1' land / '0' water; cnt islands (land joined in 4 dirs)
// (2) LC 305: all water, cells turn into land one by one; island cnt after each add
//
// clarify:
// 4 or 8 dirs -> 4; 8 is a dirs change
// grid size -> large: BFS, no deep recursion
// may modify the grid -> yes (takes a copy), sink land to '0'
// (2) pos added twice -> no change, repeat the cnt

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

// (1) count islands (LC 200)
// core idea: each unvisited land cell starts an island; BFS sinks it to '0', mark on push
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
            // q = land cells of this island still to expand
            queue<pair<int, int>> q;
            q.push({r, c});
            // mark on push -> no cell queued twice
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

// root of cell id, w/ path halving
int findRoot(vector<int>& parent, int id) {
    while (parent[id] != id) {
        parent[id] = parent[parent[id]];
        id = parent[id];
    }
    return id;
}

// (2) island cnt after each add (LC 305)
// core idea: union-find on id r * n + c; cnt++ per new land, cnt-- per union of 2 roots
// T: O(m * n + k log(m * n)), S: O(m * n) — k = positions; halving only, no rank
vector<int> numIslandsAfterAdds(int m, int n, const vector<vector<int>>& positions) {
    // parent[id] = union-find parent of land id; -1 = water
    vector<int> parent(m * n, -1);
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int count = 0;
    vector<int> result;
    for (const auto& pos : positions) {
        int id = pos[0] * n + pos[1];
        // already land -> nothing changes
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

// follow-up:
// max area (LC 695) -> same BFS, cnt cells of each flood, keep the max
// enclosed regions (LC 1254) -> flood border '0' regions to '1', cnt '0' regions left
// battleships (LC 419) -> cnt only 'X' cells w/ no 'X' above or to the left
// diagonal connectivity -> 8 entries in dirs, loop d < 8
// (2) by BFS per add -> O(k * m * n), too slow; union-find is the expected answer
// connected crop garden -> 59
