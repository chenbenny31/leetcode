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

// root of cell id, halving the path on the way up
int findRoot(vector<int>& parent, int id) {
    while (parent[id] != id) {
        parent[id] = parent[parent[id]];
        id = parent[id];
    }
    return id;
}

// LC 305: union-find on id r * n + c; count++ per new land, count-- per union that
// joins two islands. T: O(m*n + k log(m*n)), S: O(m*n); k = number of positions.
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

// BFS from (r, c) over cells equal to from, turning them into to; returns the cell count
int floodFill(vector<vector<char>>& grid, int r, int c, char from, char to) {
    int m = grid.size();
    int n = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int area = 1;
    // cells of this region still to expand
    queue<pair<int, int>> q;
    q.push({r, c});
    grid[r][c] = to;
    while (!q.empty()) {
        auto [curR, curC] = q.front();
        q.pop();
        for (int d = 0; d < 4; d++) {
            int nxtR = curR + dirs[d][0];
            int nxtC = curC + dirs[d][1];
            if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
            if (grid[nxtR][nxtC] != from) { continue; }
            grid[nxtR][nxtC] = to;
            q.push({nxtR, nxtC});
            area++;
        }
    }
    return area;
}

// LC 695: BFS each island, sinking it, and keep the largest cell count; 0 if no land.
// T: O(m*n), S: O(m*n).
int maxAreaOfIsland(vector<vector<char>> grid) {
    int m = grid.size();
    int n = grid[0].size();
    int best = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] != '1') { continue; }
            int area = floodFill(grid, r, c, '1', '0');
            if (area > best) { best = area; }
        }
    }
    return best;
}

// Google (LC 1254 style): count the '0' regions that touch no border cell ('1' walls
// them in). Fill every '0' region on the border into '1', then each '0' left starts an
// enclosed region. T: O(m*n), S: O(m*n).
int countEnclosedRegions(vector<vector<char>> grid) {
    int m = grid.size();
    int n = grid[0].size();
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            bool border = r == 0 || r == m - 1 || c == 0 || c == n - 1;
            if (border && grid[r][c] == '0') { floodFill(grid, r, c, '0', '1'); }
        }
    }
    int regions = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] != '0') { continue; }
            regions++;
            floodFill(grid, r, c, '0', '1');
        }
    }
    return regions;
}

// LC 419: ships are 1 x k or k x 1 runs of 'X', never adjacent. Count each ship once at
// its top-left cell: an 'X' with no 'X' above and none to the left. T: O(m*n), S: O(1).
int countBattleships(const vector<vector<char>>& board) {
    int m = board.size();
    int n = board[0].size();
    int ships = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (board[r][c] != 'X') { continue; }
            if (r > 0 && board[r - 1][c] == 'X') { continue; }
            if (c > 0 && board[r][c - 1] == 'X') { continue; }
            ships++;
        }
    }
    return ships;
}

void test(const string& label, const vector<vector<char>>& grid, int want) {
    cout << label << (numIslands(grid) == want ? " PASS\n" : " FAIL\n");
}

void testAdds(const string& label, int m, int n, const vector<vector<int>>& positions,
              const vector<int>& want) {
    bool ok = numIslandsAfterAdds(m, n, positions) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testArea(const string& label, const vector<vector<char>>& grid, int want) {
    cout << label << (maxAreaOfIsland(grid) == want ? " PASS\n" : " FAIL\n");
}

void testEnclosed(const string& label, const vector<vector<char>>& grid, int want) {
    cout << label << (countEnclosedRegions(grid) == want ? " PASS\n" : " FAIL\n");
}

void testShips(const string& label, const vector<vector<char>>& board, int want) {
    cout << label << (countBattleships(board) == want ? " PASS\n" : " FAIL\n");
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
    testArea("area-bigger-wins", {{'1', '0', '1'},
                                  {'0', '0', '1'},
                                  {'0', '1', '1'}}, 4);
    testArea("area-full", {{'1', '1'}, {'1', '1'}}, 4);
    testArea("area-all-water", {{'0'}}, 0);
    testEnclosed("enclosed-one-lake", {{'1', '1', '1'},
                                       {'1', '0', '1'},
                                       {'1', '1', '1'}}, 1);
    testEnclosed("enclosed-touches-border", {{'1', '1', '1'},
                                             {'1', '0', '0'},
                                             {'1', '1', '1'}}, 0);
    testEnclosed("enclosed-diagonal-two", {{'1', '1', '1', '1'},
                                           {'1', '0', '1', '1'},
                                           {'1', '1', '0', '1'},
                                           {'1', '1', '1', '1'}}, 2);
    testShips("ships-horizontal-vertical", {{'X', 'X', '.'},
                                            {'.', '.', 'X'},
                                            {'.', '.', 'X'}}, 2);
    testShips("ships-single-cells", {{'X', '.', 'X'}}, 2);
    testShips("ships-none", {{'.'}}, 0);
}

// follow-up: diagonal connectivity -> 8 entries in dirs (add the 4 diagonals)
