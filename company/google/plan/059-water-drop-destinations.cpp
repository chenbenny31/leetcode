// Water Drop Destinations (Google, interview report)
// (1) drop moves to its lowest strictly-lower 4-neighbor, else stops; stop cell per cell
// (2) very large grid, no recursion
//
// clarify:
// equal neighbor -> drop stops (strictly lower only)
// ties among the lowest -> first of up, down, left, right
// no lower neighbor -> cell is its own destination
// grid size -> recursion depth; follow-up (2)

#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

using Cell = pair<int, int>;

// lowest neighbor strictly lower than (r, c), or (r, c) itself if none
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

// (1) solution 1: simulate every drop; baseline
// core idea: follow each drop step by step until it stops
// T: O((mn)^2), S: O(mn) — a path can visit every cell
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

// dest of (r, c) = dest of the cell its drop moves to; unset cells hold {-1, -1}
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

// (1) solution 2: memoized DFS; each cell resolved once
// core idea: one next cell per cell, heights strictly drop -> no cycles; dest = next's
// T: O(mn), S: O(mn)
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

// (2) iterative, no recursion
// core idea: follow the drop, keeping passed cells in path, until a cell w/ known dest
// then give that dest to the whole path
// T: O(mn), S: O(mn)
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
    test("lowest-wins", {{1, 3, 5, 2, 4}},
         {{{0, 0}, {0, 0}, {0, 3}, {0, 3}, {0, 3}}});
    test("flat", {{5, 5}, {5, 5}}, {{{0, 0}, {0, 1}}, {{1, 0}, {1, 1}}});
    test("tie-up-first", {{1}, {5}, {1}}, {{{0, 0}}, {{0, 0}}, {{2, 0}}});
    test("one-sink", {{9, 8, 7}, {4, 5, 6}, {3, 2, 1}},
         {{{2, 2}, {2, 2}, {2, 2}}, {{2, 2}, {2, 2}, {2, 2}}, {{2, 2}, {2, 2}, {2, 2}}});
}

// follow-up:
// how many drops end in each sink -> cnt the dest cells in a map Cell -> cnt
// drops may move onto equal heights (plateaus) -> moves can cycle; agree on a rule first
// plateau rule -> BFS each flat basin as one node, or mark cells "in progress"
