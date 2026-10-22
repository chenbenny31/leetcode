// LC 417 Pacific Atlantic Water Flow: water flows from a cell to a 4-neighbor of equal
// or lower height. Pacific touches the top and left edges, Atlantic the bottom and
// right. Return all cells [r, c] from which water reaches both oceans.
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

// Reverse the flow: BFS from one ocean's edge cells, moving to equal or higher cells.
// T: O(R*C), S: O(R*C).
vector<vector<bool>> reach(const vector<vector<int>>& heights, bool pacific) {
    int m = heights.size(), n = heights[0].size();
    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    vector<vector<bool>> seen(m, vector<bool>(n, false));   // cells that drain here
    queue<pair<int, int>> q;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            bool edge = pacific ? (r == 0 || c == 0) : (r == m - 1 || c == n - 1);
            if (!edge) { continue; }
            seen[r][c] = true;
            q.push({r, c});
        }
    }
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (const auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            if (seen[nr][nc] || heights[nr][nc] < heights[r][c]) { continue; }
            seen[nr][nc] = true;
            q.push({nr, nc});
        }
    }
    return seen;
}

// Cells drained by both oceans, in row-major order.
vector<vector<int>> pacificAtlantic(const vector<vector<int>>& heights) {
    int m = heights.size(), n = heights[0].size();
    vector<vector<bool>> pac = reach(heights, true);
    vector<vector<bool>> atl = reach(heights, false);
    vector<vector<int>> both;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (pac[r][c] && atl[r][c]) { both.push_back({r, c}); }
        }
    }
    return both;
}

void test(const string& label, const vector<vector<int>>& heights,
          const vector<vector<int>>& want) {
    cout << label << (pacificAtlantic(heights) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {{5}}, {{0, 0}});
    test("slope", {{1, 2}, {4, 3}}, {{0, 1}, {1, 0}, {1, 1}});
    test("valley", {{5, 5, 5}, {5, 1, 5}, {5, 5, 5}},
         {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 2}, {2, 0}, {2, 1}, {2, 2}});
}
