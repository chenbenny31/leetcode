// LC 1631 Path With Minimum Effort: heights grid, move in 4 directions from top-left to
// bottom-right. A path's effort is the maximum absolute height difference between
// consecutive cells. Return the minimum effort over all paths.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include <cstdlib>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::greater;
using std::abs;
using std::max;

// Dijkstra where a path costs its maximum edge instead of the sum (minimax path).
// T: O(m*n log(m*n)), S: O(m*n).
int minimumEffortPath(vector<vector<int>>& heights) {
    int m = heights.size(), n = heights[0].size();
    vector<vector<int>> effort(m, vector<int>(n, INT_MAX));   // best known effort to cell
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    effort[0][0] = 0;
    pq.push({0, 0});   // (effort, cell id r * n + c), lowest effort first
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    while (!pq.empty()) {
        auto [e, id] = pq.top();
        pq.pop();
        int r = id / n, c = id % n;
        if (e > effort[r][c]) { continue; }
        if (r == m - 1 && c == n - 1) { return e; }
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            int ne = max(e, abs(heights[nr][nc] - heights[r][c]));
            if (ne < effort[nr][nc]) {
                effort[nr][nc] = ne;
                pq.push({ne, nr * n + nc});
            }
        }
    }
    return 0;
}

void test(const string& label, vector<vector<int>> heights, int want) {
    cout << label << (minimumEffortPath(heights) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("flat-detour", {{1, 1, 1}, {9, 9, 1}, {1, 1, 1}}, 0);
    test("forced-climb", {{1, 5}, {9, 4}}, 4);
    test("single-cell", {{7}}, 0);
}
