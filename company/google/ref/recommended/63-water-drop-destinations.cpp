// Water Drop Destinations (Google, interview report): an m x n grid of heights. A drop on
// a cell moves to its lowest 4-neighbor if that neighbor is strictly lower than the
// current cell, else it stops. Ties among the lowest neighbors: first in the order up,
// down, left, right. Return for every cell the (row, col) where its drop stops.
//
// Clarify:
// - "strictly lower" -> an equal neighbor stops the drop (plateaus are a follow-up)
// - ties among the lowest neighbors -> first in up, down, left, right order (stated)
// - a cell with no lower neighbor is its own destination
// - up to ~1e3 x 1e3 cells -> simulating every drop is O((mn)^2) on a long slope;
//   memoize to O(mn)
// - return (row, col) for every cell, as an m x n grid
// Idea: each cell has at most one next cell (its lowest strictly lower neighbor), and
//   heights strictly drop along the moves, so the moves form a forest rooted at the
//   sinks. A cell's destination is its next cell's destination: memoize and each cell
//   is resolved once.
// Pitfalls:
// - start best at (r, c) and compare with heights[best]: only strictly lower neighbors
//   win, and the lowest of them is kept
// - strict < keeps the earlier direction on ties, so dirs must be up, down, left, right
// - the memo sentinel is {-1, -1}; (0, 0) is a real destination, never "unknown"
// - the DFS recurses up to m*n deep on a long slope (~1e6 cells overflows the stack):
//   iterative follow-up
// Tests to write: a slope; the lower of two lower neighbors wins; flat grid (all
//   sinks); tie -> up first; two-step chain; a 3 x 3 grid draining into one sink.

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

// follow-up (very large grid): no recursion. Follow the drop from each cell, keeping
// the cells passed in path, until a cell whose dest is known (a sink sets its own);
// then give every cell on the path that dest. Each cell joins one path, so
// T: O(mn), S: O(mn).
vector<vector<Cell>> dropDestinationsIterative(const vector<vector<int>>& heights) {
    int m = heights.size();
    int n = heights[0].size();
    // dest[r][c] = where the drop from (r, c) stops, {-1, -1} until known
    vector<vector<Cell>> dest(m, vector<Cell>(n, {-1, -1}));
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            // cells passed by this drop whose dest is not known yet
            vector<Cell> path;
            Cell cur = {r, c};
            while (dest[cur.first][cur.second].first == -1) {
                path.push_back(cur);
                Cell nxt = lowestNeighbor(heights, cur.first, cur.second);
                if (nxt == cur) {
                    // a sink is its own destination; this also ends the loop
                    dest[cur.first][cur.second] = cur;
                } else {
                    cur = nxt;
                }
            }
            Cell stop = dest[cur.first][cur.second];
            for (Cell cell : path) { dest[cell.first][cell.second] = stop; }
        }
    }
    return dest;
}

void test(const string& label, const vector<vector<int>>& heights,
          const vector<vector<Cell>>& want) {
    bool ok = dropDestinations(heights) == want && dropDestinationsFast(heights) == want;
    ok = ok && dropDestinationsIterative(heights) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("slope", {{3, 2, 1}}, {{{0, 2}, {0, 2}, {0, 2}}});
    test("lowest-wins", {{1, 3, 5, 2, 4}},
         {{{0, 0}, {0, 0}, {0, 3}, {0, 3}, {0, 3}}});
    test("flat", {{5, 5}, {5, 5}}, {{{0, 0}, {0, 1}}, {{1, 0}, {1, 1}}});
    test("tie-up-first", {{1}, {5}, {1}}, {{{0, 0}}, {{0, 0}}, {{2, 0}}});
    test("chain", {{4, 3}, {1, 2}}, {{{1, 0}, {1, 0}}, {{1, 0}, {1, 0}}});
    test("one-sink", {{9, 8, 7}, {4, 5, 6}, {3, 2, 1}},
         {{{2, 2}, {2, 2}, {2, 2}}, {{2, 2}, {2, 2}, {2, 2}}, {{2, 2}, {2, 2}, {2, 2}}});
}

// follow-up: how many drops end in each sink -> count the dest cells:
//   map<Cell, int> drops;
//   for (const vector<Cell>& row : dest) {
//       for (Cell cell : row) { drops[cell]++; }
//   }
// follow-up: drops may also move onto equal heights (plateaus) -> agree on the rule
//   first: moves can now cycle on a plateau, so the memo DFS would loop forever; mark
//   cells "in progress" and treat a cycle (a flat basin) as one sink, or BFS each
//   plateau as one node
// follow-up: very large grid (deep recursion) -> dropDestinationsIterative above
