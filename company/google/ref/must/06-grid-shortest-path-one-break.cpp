// Grid shortest paths (Google, interview report). grid[r][c] = 0 passable, 1 blocked;
// moves go up / down / left / right.
// (1) Return the cells of one shortest path from any passable cell in column 0 to any
//     passable cell in column n - 1 (a single cell if n == 1); empty if none.
// (2) Fewest moves from (0, 0) to (m - 1, n - 1) over passable cells, -1 if none.
// (3) Same as (2), but at most one blocked cell may be removed (start included).
//
// Clarify:
// - 0 passable / 1 blocked, 4 directions
// - (1) any shortest path if several; a single cell when n == 1
// - (3) "at most one" removal (not exactly one); a blocked start or target uses it
// - grid size up to ~1e3 x 1e3 -> O(m*n) BFS
// Idea: (1) BFS from every passable cell of column 0 at once, parent per cell, rebuild
//   the path from the first cell popped in the last column. (2) plain BFS with dist.
//   (3) BFS over states (r, c, used): the same cell reached with the removal still
//   available is a different, better state than reaching it with the removal spent.
// Pitfalls:
// - (3) seen per (cell, used), not per cell: a cell first reached by spending the
//   removal must still be enterable later by a path that has not spent it
// - (3) a blocked start begins with used = 1
// - (1) parent stores r * n + c; decode with / n and % n (n = columns)
// - (2) blocked start -> -1 before the BFS
// Tests to write: (1) straight row, winding path, one column, blocked; (2) open,
//   detour, walled, single cell; (3) a break shortens a snake, a full wall row, two
//   walls in a row -> -1, open grid needs no break.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::reverse;

const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// (1) multi-source BFS from every passable cell of column 0; the first cell popped in
// the last column ends a shortest path, rebuilt from parents. T: O(m*n), S: O(m*n).
vector<pair<int, int>> pathAcross(const vector<vector<int>>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    // parent[r][c] = r * n + c of the cell we came from, -1 for a start cell
    vector<vector<int>> parent(m, vector<int>(n, -1));
    vector<vector<bool>> seen(m, vector<bool>(n, false));
    queue<pair<int, int>> q;
    for (int r = 0; r < m; r++) {
        if (grid[r][0] == 1) { continue; }
        seen[r][0] = true;
        q.push({r, 0});
    }
    while (!q.empty()) {
        auto [curR, curC] = q.front();
        q.pop();
        if (curC == n - 1) {
            vector<pair<int, int>> path;
            int code = curR * n + curC;
            while (code != -1) {
                path.push_back({code / n, code % n});
                code = parent[code / n][code % n];
            }
            reverse(path.begin(), path.end());
            return path;
        }
        for (int d = 0; d < 4; d++) {
            int nxtR = curR + dirs[d][0];
            int nxtC = curC + dirs[d][1];
            if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
            if (grid[nxtR][nxtC] == 1 || seen[nxtR][nxtC]) { continue; }
            seen[nxtR][nxtC] = true;
            parent[nxtR][nxtC] = curR * n + curC;
            q.push({nxtR, nxtC});
        }
    }
    return {};
}

// (2) BFS from (0, 0); dist set when a cell is first queued. T: O(m*n), S: O(m*n).
int shortestPath(const vector<vector<int>>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    if (grid[0][0] == 1) { return -1; }
    // dist[r][c] = moves from (0, 0), -1 if not reached yet
    vector<vector<int>> dist(m, vector<int>(n, -1));
    dist[0][0] = 0;
    queue<pair<int, int>> q;
    q.push({0, 0});
    while (!q.empty()) {
        auto [curR, curC] = q.front();
        q.pop();
        for (int d = 0; d < 4; d++) {
            int nxtR = curR + dirs[d][0];
            int nxtC = curC + dirs[d][1];
            if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
            if (grid[nxtR][nxtC] == 1 || dist[nxtR][nxtC] != -1) { continue; }
            dist[nxtR][nxtC] = dist[curR][curC] + 1;
            q.push({nxtR, nxtC});
        }
    }
    return dist[m - 1][n - 1];
}

struct State {
    int r;
    int c;
    // 1 once the one removal is spent
    int used;
};

// (3) BFS by layers over (r, c, used): a blocked cell is enterable only while used == 0,
// and entering it spends the removal. T: O(m*n), S: O(m*n).
int shortestPathOneBreak(const vector<vector<int>>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    // seen[r][c][used] = state already queued
    vector<vector<vector<bool>>> seen(m, vector<vector<bool>>(n, vector<bool>(2)));
    int startUsed = grid[0][0];
    seen[0][0][startUsed] = true;
    queue<State> q;
    q.push({0, 0, startUsed});
    int steps = 0;
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++) {
            State cur = q.front();
            q.pop();
            if (cur.r == m - 1 && cur.c == n - 1) { return steps; }
            for (int d = 0; d < 4; d++) {
                int nxtR = cur.r + dirs[d][0];
                int nxtC = cur.c + dirs[d][1];
                if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
                int nxtUsed = cur.used + grid[nxtR][nxtC];
                if (nxtUsed > 1 || seen[nxtR][nxtC][nxtUsed]) { continue; }
                seen[nxtR][nxtC][nxtUsed] = true;
                q.push({nxtR, nxtC, nxtUsed});
            }
        }
        steps++;
    }
    return -1;
}

void testAcross(const string& label, const vector<vector<int>>& grid,
                const vector<pair<int, int>>& want) {
    cout << label << (pathAcross(grid) == want ? " PASS\n" : " FAIL\n");
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (shortestPath(grid) == want ? " PASS\n" : " FAIL\n");
}

void testBreak(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (shortestPathOneBreak(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    testAcross("straight", {{0, 0, 0}}, {{0, 0}, {0, 1}, {0, 2}});
    testAcross("winding", {{0, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 0, 0}},
               {{0, 0}, {0, 1}, {1, 1}, {2, 1}, {2, 2}, {2, 3}});
    testAcross("one-column", {{1}, {0}}, {{1, 0}});
    testAcross("blocked", {{0, 1}, {0, 1}}, {});

    test("open", {{0, 0}, {0, 0}}, 2);
    test("detour", {{0, 0, 0}, {1, 1, 0}, {0, 0, 0}}, 4);
    test("walled", {{0, 1}, {1, 0}}, -1);
    test("single", {{0}}, 0);

    testBreak("snake", {{0, 0, 0}, {1, 1, 0}, {0, 0, 0}, {0, 1, 1}, {0, 0, 0}}, 6);
    testBreak("wall-row", {{0, 0}, {1, 1}, {0, 0}}, 3);
    testBreak("two-walls", {{0, 1, 1, 0}}, -1);
    testBreak("no-need", {{0, 0}, {0, 0}}, 2);
}

// follow-up: up to k removals (LC 1293) -> the same BFS with used = 0..k:
//   vector<vector<vector<bool>>> seen(m, vector<vector<bool>>(n, vector<bool>(k + 1)));
//   int nxtUsed = cur.used + grid[nxtR][nxtC];
//   if (nxtUsed > k || seen[nxtR][nxtC][nxtUsed]) { continue; }
//   T: O(m*n*k). If k >= m + n - 1 (every cell of a straight path, start included),
//   the answer is m + n - 2 without a BFS (LC 1293 itself, with start and target
//   always free, needs only k >= m + n - 3)
// follow-up: (1) fewest cells to unblock to connect the sides -> 0-1 BFS on cost =
//   blocked cells passed: seed every column-0 cell with cost grid[r][0]; a step into a
//   free cell (cost + 0) goes to the front of a deque, into a blocked one (cost + 1) to
//   the back; pop from the front and skip entries whose cost is stale
