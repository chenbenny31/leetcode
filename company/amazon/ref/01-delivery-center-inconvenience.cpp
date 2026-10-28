// An m x n grid has 1 = delivery center and 0 = location. Distance is Chebyshev:
// max(|r1 - r2|, |c1 - c2|). The inconvenience is the max, over all 0 cells, of the
// distance to the nearest center. You may turn at most one 0 into a 1. Return the
// minimum possible inconvenience.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::max;
using std::min;

// multi-source BFS with 8 moves: one step of Chebyshev distance per BFS layer
vector<vector<int>> nearestCenter(const vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size();
    // dist[r][c] = distance from (r, c) to the nearest center, INT_MAX if there is none
    vector<vector<int>> dist(m, vector<int>(n, INT_MAX));
    queue<pair<int, int>> q;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 1) {
                dist[r][c] = 0;
                q.push({r, c});
            }
        }
    }
    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop();
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                int nr = cur.first + dr, nc = cur.second + dc;
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                if (dist[nr][nc] != INT_MAX) { continue; }
                dist[nr][nc] = dist[cur.first][cur.second] + 1;
                q.push({nr, nc});
            }
        }
    }
    return dist;
}

// Can one new center reach every cell whose distance is > d? Those cells fit in one
// (2d+1) x (2d+1) square iff their row span and column span are both <= 2d.
bool coverable(const vector<vector<int>>& dist, int d) {
    int m = dist.size(), n = dist[0].size();
    int minR = m, maxR = -1, minC = n, maxC = -1;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (dist[r][c] > d) {
                minR = min(minR, r);
                maxR = max(maxR, r);
                minC = min(minC, c);
                maxC = max(maxC, c);
            }
        }
    }
    if (maxR == -1) { return true; }
    return maxR - minR <= 2 * d && maxC - minC <= 2 * d;
}

// straightforward: BFS, then try d = 0, 1, 2, ... until one new center is enough.
// T: O(mn * D) for answer D, S: O(mn).
int minInconvenience(const vector<vector<int>>& grid) {
    vector<vector<int>> dist = nearestCenter(grid);
    int d = 0;
    while (!coverable(dist, d)) { d++; }
    return d;
}

// improved: coverable(d) is monotone in d, so binary search it.
// T: O(mn log(m + n)), S: O(mn).
int minInconvenienceBinary(const vector<vector<int>>& grid) {
    vector<vector<int>> dist = nearestCenter(grid);
    int lo = 0, hi = max(grid.size(), grid[0].size());
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (coverable(dist, mid)) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    bool ok = minInconvenience(grid) == want && minInconvenienceBinary(grid) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-row", {{0, 0, 0, 1}}, 1);
    test("both-sides", {{0, 0, 0, 1, 0, 0, 0}}, 3);
    test("no-center", {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 1);
    test("one-zero", {{1, 0}}, 0);
    test("all-centers", {{1, 1}}, 0);
}
