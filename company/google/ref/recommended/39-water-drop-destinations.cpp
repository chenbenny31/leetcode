// Water Drop Destinations (Google, interview report): an m x n grid of heights. A drop on
// a cell moves to its lowest 4-neighbor if that neighbor is strictly lower than the
// current cell, else it stops. Ties among the lowest neighbors: first in the order up,
// down, left, right. Return for every cell the (row, col) where its drop stops.

#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

using Cell = pair<int, int>;

// lowest neighbor strictly lower than (r, c), or (r, c) itself if there is none
Cell lowestNeighbor(const vector<vector<int>>& heights, int r, int c) {
    int m = heights.size();
    int n = heights[0].size();
    // up, down, left, right
    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    Cell best = {r, c};
    for (auto& d : dirs) {
        int nr = r + d[0];
        int nc = c + d[1];
        if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
        // strict < keeps the earlier direction among equal neighbors
        if (heights[nr][nc] < heights[best.first][best.second]) { best = {nr, nc}; }
    }
    return best;
}

// simulate every drop step by step. T: O((mn)^2) worst (a path can visit every cell),
// S: O(mn) for the answer.
vector<vector<Cell>> dropDestinations(const vector<vector<int>>& heights) {
    int m = heights.size();
    int n = heights[0].size();
    vector<vector<Cell>> dest(m, vector<Cell>(n));
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            Cell cur = {r, c};
            Cell nxt = lowestNeighbor(heights, r, c);
            while (nxt != cur) {
                cur = nxt;
                nxt = lowestNeighbor(heights, cur.first, cur.second);
            }
            dest[r][c] = cur;
        }
    }
    return dest;
}

// dest[r][c] = dest of the cell the drop moves to; heights strictly drop along a path,
// so there are no cycles. Unset cells hold {-1, -1}.
Cell dfs(const vector<vector<int>>& heights, int r, int c, vector<vector<Cell>>& dest) {
    if (dest[r][c].first != -1) { return dest[r][c]; }
    Cell nxt = lowestNeighbor(heights, r, c);
    if (nxt == Cell{r, c}) {
        dest[r][c] = nxt;
    } else {
        dest[r][c] = dfs(heights, nxt.first, nxt.second, dest);
    }
    return dest[r][c];
}

// memoized DFS: each cell is resolved once. T: O(mn), S: O(mn).
vector<vector<Cell>> dropDestinationsFast(const vector<vector<int>>& heights) {
    int m = heights.size();
    int n = heights[0].size();
    // dest[r][c] = where the drop from (r, c) stops, {-1, -1} until known
    vector<vector<Cell>> dest(m, vector<Cell>(n, {-1, -1}));
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) { dfs(heights, r, c, dest); }
    }
    return dest;
}

void test(const string& label, const vector<vector<int>>& heights,
          const vector<vector<Cell>>& want) {
    bool ok = dropDestinations(heights) == want && dropDestinationsFast(heights) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("slope", {{3, 2, 1}}, {{{0, 2}, {0, 2}, {0, 2}}});
    test("lowest-wins", {{1, 3, 5, 2, 4}},
         {{{0, 0}, {0, 0}, {0, 3}, {0, 3}, {0, 3}}});
    test("flat", {{5, 5}, {5, 5}}, {{{0, 0}, {0, 1}}, {{1, 0}, {1, 1}}});
    test("tie-up-first", {{1}, {5}, {1}}, {{{0, 0}}, {{0, 0}}, {{2, 0}}});
    test("chain", {{4, 3}, {1, 2}}, {{{1, 0}, {1, 0}}, {{1, 0}, {1, 0}}});
}

// follow-up: how many drops end in each sink -> count dest cells: map<Cell, int>
// follow-up: drop may also move onto equal heights (plateaus) -> paths can cycle; BFS
//   over each plateau as one node, or mark cells "in progress" in the DFS
// follow-up: very large grid (deep recursion) -> follow the path into a stack until a
//   known or stopping cell, then assign its dest to every cell on the stack
