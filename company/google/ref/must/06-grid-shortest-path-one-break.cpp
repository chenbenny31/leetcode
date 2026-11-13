// Grid shortest paths (Google, interview report)
// grid[r][c] = 0 passable, 1 blocked; moves up / down / left / right
// (1) cells of one shortest path from any passable cell in col 0 to col n - 1; {} if none
// (2) fewest moves (0, 0) -> (m - 1, n - 1) over passable cells, -1 if none
// (3) same as (2), but at most one blocked cell may be removed (start included)
//
// clarify:
// several shortest paths -> any one
// n == 1 -> path = one passable cell of col 0
// "at most one" removal -> blocked start or target uses it
// grid size -> BFS is O(m * n), fine

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

// (1) one shortest path across
// core idea: multi-source BFS from all passable cells of col 0
// first cell popped in last col ends a shortest path; rebuild from parents
// T: O(m * n), S: O(m * n)
vector<pair<int, int>> pathAcross(const vector<vector<int>>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    // parent[r][c] = r * n + c of the cell we came from; -1 = start cell
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

// (2) fewest moves corner to corner
// core idea: BFS from (0, 0); dist set when a cell is first queued
// T: O(m * n), S: O(m * n)
int shortestPath(const vector<vector<int>>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    if (grid[0][0] == 1) { return -1; }
    // dist[r][c] = moves from (0, 0); -1 = not reached yet
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
    // used = 1 once the one removal is spent
    int used;
};

// (3) fewest moves w/ one wall break
// core idea: BFS by layers over states (r, c, used); seen per state, not per cell
// blocked cell is enterable only while used == 0; entering it spends the removal
// T: O(m * n), S: O(m * n)
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

// follow-up:
// up to k removals (LC 1293) -> same BFS w/ used = 0..k, seen[r][c][used]; O(m * n * k)
// k >= m + n - 1 -> answer is m + n - 2, no BFS
// (1) fewest cells to unblock to connect the sides -> 0-1 BFS on cost = blocked cells
// 0-1 BFS -> deque: free step to the front, blocked step (cost + 1) to the back
