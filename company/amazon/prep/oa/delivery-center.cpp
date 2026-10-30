// mxn grid, 1=delivery center, 0=location, dist by max(delta(r/c))
// return the min possible inconvenience, can turn at most one 0 into 1

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

// multi-source BFS, 8 moves,
vector<vector<int>> nearestCenter(const vector<vector<int>>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    // dist[r][c] = dist from (r,c) to nearest center, INT_MAX default
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
        auto [r, c] = q.front();
        q.pop();
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                int nr = r + dr;
                int nc = c + dc;
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                if (dist[nr][nc] != INT_MAX) { continue; }
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
    return dist;
} 

// can one center reach every cell with dist > d?
bool coverable(const vector<vector<int>>& dist, int d) {
    int m = dist.size();
    int n = dist[0].size();
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
    if (maxR == -1) { return true; } // already cover
    // set a new center to test coverage
    int pr = (maxR + minR) / 2;
    int pc = (minC + maxC) / 2;
    return max(maxR - pr, pr - minR) <= d
        && max(maxC - pc, pc - minC) <= d;
}

// BFS, T: O(mn*D), S: O(mn)
int minInconvenience(const vector<vector<int>>& grid) {
    vector<vector<int>> dist = nearestCenter(grid);
    int d = 0;
    while (!coverable(dist, d)) { d++; }
    return d;
}

// BFS + binary-search, coverable(d) is mono, T:O(mn log(m+n)), S: O(mn)
int minInconvenienceBinary(const vector<vector<int>>& grid) {
    vector<vector<int>> dist = nearestCenter(grid);
    int lo = 0;
    int hi = max(grid.size(), grid[0].size());
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
    bool ok = minInconvenience(grid) == want
        && minInconvenienceBinary(grid) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-row", {{0, 0, 0, 1}}, 1);
    test("both-sides", {{0, 0, 0, 1, 0, 0, 0}}, 3);
    test("no-center", {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 1);
    test("one-zero", {{1, 0}}, 0);
    test("all-centers", {{1, 1}}, 0);
}
