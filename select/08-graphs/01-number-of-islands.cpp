// LC 200 Number of Islands (Medium). Pattern: grid BFS flood fill.
// grid of '1' (land) and '0' (water); count islands, where an island is land connected
// horizontally or vertically.
//
// Clarify:
// - cells are chars '1' land / '0' water -> assume so
// - connectivity: 4 directions only (diagonal is a follow-up)
// - empty grid? -> assume at least 1 x 1 (else return 0 before reading grid[0])
// - may I modify the input? -> sink visited land to '0'; here the function takes a copy
// - size up to ~300 x 300 -> a recursive DFS can go 9e4 calls deep; use BFS
// Idea: every land cell not visited yet starts a new island; BFS from it sinks the whole
//   island, marking on push so no cell is queued twice. Each cell is pushed at most once.
// Pitfalls:
// - mark on push, not on pop (a cell reached from two sides gets queued twice)
// - bounds check before reading grid[nxtR][nxtC]
// - auto [curR, curC] = q.front() is a copy taken before pop(), never auto&
// Tests to write: two islands; U-shape is one island; diagonal cells are two islands;
//   all water -> 0.

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

// LC 305 (Google: land added one by one ×3): an m x n grid of water; each position turns
// a cell into land; return the island count after each add.
// Clarify: repeated position -> count unchanged, still output it; positions in range;
//   4 directions; m*n and k up to ~1e4, so a BFS per add (O(k*m*n)) is too slow.
// Idea: union-find on id r * n + c (parent -1 = water); count++ per new land, count--
//   per union that joins two different roots.
// Pitfalls: id is r * n + c (n = columns); push the count on a repeated position too;
//   find both roots inside the loop (an earlier union moves them); link root to root.
// Tests to write: 1 x 3 bridge -> 1, 2, 1; 2 x 2 diagonal then ring -> 1, 2, 1, 1;
//   repeated position -> 1, 1.
// T: O(m*n + k log(m*n)), S: O(m*n); k = number of positions. Union by size on top
// makes each find ~O(1).
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
// Pitfall: count a cell when it is pushed (area starts at 1 for the seed).
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
// Pitfall: start the border fill from every border cell, not only the corners; two '0'
// cells touching only diagonally are two regions.
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
// Pitfall: check r > 0 / c > 0 before reading the cell above / to the left.
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

// follow-up: diagonal connectivity (Google ×2) -> 8 entries in dirs, loop d < 8:
//   int dirs[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1},
//                     {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
