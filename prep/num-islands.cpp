// 4 or 8 dirs
// gris-size be large -> BFS no deep recursion
// allow in-place modify -> (copy)

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
using std::swap;

// each un-visited starts an island, BFS sinks it to '0', mark on push, T: O(mn), S: O(mn)
int numIslands(vector<vector<char>> grid) {
    int m = grid.size();
    int n = grid[0].size();
    int dr[] = {0, 0, +1, -1};
    int dc[] = {+1, -1, 0, 0};
    int islands = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] != '1') { continue; }
            islands++;
            queue<pair<int, int>> q;
            q.push({r, c});
            grid[r][c] = '0';
            while (!q.empty()) {
                auto [cr, cc] = q.front();
                q.pop();
                for (int d = 0; d < 4; d++) {
                    int nr = cr + dr[d];
                    int nc = cc + dc[d];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                    if (grid[nr][nc] != '1') { continue; }
                    grid[nr][nc] = '0'; // mark on push
                    q.push({nr, nc});
                }
            }
        }
    }
    return islands;
}

int find(vector<int>& parent, int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x]; 
    }
    return x;
}

bool unite(vector<int>& parent, vector<int>& sz, int a, int b) {
    int rootA = find(parent, a);
    int rootB = find(parent, b);
    if (rootA == rootB) { return false; }
    if (sz[rootA] < sz[rootB]) { swap(rootA, rootB); }
    parent[rootB] = rootA;
    sz[rootA] += sz[rootB];
    return true;
}

// union-find on id=r*n+c, cnt++ per new land, cnt-- per union, T: O(mn+k), S: O(mn), k = positions, ~O(1) per op
vector<int> numIslandsAfterAdds(int m, int n, const vector<vector<int>>& positions) {
    vector<int> parent(m * n, -1);
    vector<int> sz(m * n, 1);
    int dr[] = {0, 0, +1, -1};
    int dc[] = {+1, -1, 0, 0};
    int count = 0;
    vector<int> result;
    for (const auto& pos : positions) {
        int id = pos[0] * n + pos[1];
        if (parent[id] != -1) {
            result.push_back(count);
            continue;
        }

        parent[id] = id;
        count++;
        for (int d = 0; d < 4; d++) {
            int nr = pos[0] + dr[d];
            int nc = pos[1] + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            int nxtId = nr * n + nc;
            if (parent[nxtId] == -1) { continue; }
            if (unite(parent, sz, id, nxtId)) { count--; }
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

// one-island, two-island, u-shape, diag, all-water, all-bridge

// max-area: BFS + count cells of flood, keep the max
// diag-conn: 8 dirs
// per add -> uf

