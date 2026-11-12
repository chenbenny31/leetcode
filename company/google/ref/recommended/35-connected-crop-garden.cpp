// Connected Crop Garden (Google, interview report): an m x n field and crop types
// 0..k-1 with counts[crop] cells each (counts >= 0 and they sum to m * n). Assign a crop
// to every cell so that the cells of each crop form one 4-connected region. Return the
// grid of crop ids; any valid grid is accepted (a crop with count 0 is simply absent).

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;

// Walk the field in snake order (row 0 left->right, row 1 right->left, ...) and plant
// crop 0 for counts[0] cells, then crop 1, ... Two consecutive snake cells are always
// 4-adjacent: inside a row they are side by side, and a row ends in the same column
// where the next row starts, directly below it. So each crop's run is a path of
// adjacent cells, hence connected. T: O(mn), S: O(mn) for the grid.
vector<vector<int>> plantCrops(int m, int n, const vector<int>& counts) {
    // seq[pos] = crop of the pos-th cell along the snake
    vector<int> seq;
    for (int crop = 0; crop < (int)counts.size(); crop++) {
        seq.insert(seq.end(), counts[crop], crop);
    }
    vector<vector<int>> grid(m, vector<int>(n));
    int pos = 0;
    for (int r = 0; r < m; r++) {
        for (int i = 0; i < n; i++) {
            int c = r % 2 == 0 ? i : n - 1 - i;
            grid[r][c] = seq[pos];
            pos++;
        }
    }
    return grid;
}

// test helper: each crop has counts[crop] cells and one BFS from its first cell reaches
// all of them (a second unvisited cell of the same crop means a second region)
bool isValid(const vector<vector<int>>& grid, const vector<int>& counts) {
    int m = grid.size();
    int n = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // reached[crop] = cells of crop reached by its BFS
    vector<int> reached(counts.size(), 0);
    vector<vector<bool>> seen(m, vector<bool>(n, false));
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            int crop = grid[r][c];
            if (crop < 0 || crop >= (int)counts.size()) { return false; }
            if (seen[r][c]) { continue; }
            if (reached[crop] > 0) { return false; }
            seen[r][c] = true;
            queue<pair<int, int>> q;
            q.push({r, c});
            while (!q.empty()) {
                auto [cr, cc] = q.front();
                q.pop();
                reached[crop]++;
                for (auto& d : dirs) {
                    int nr = cr + d[0];
                    int nc = cc + d[1];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                    if (seen[nr][nc] || grid[nr][nc] != crop) { continue; }
                    seen[nr][nc] = true;
                    q.push({nr, nc});
                }
            }
        }
    }
    return reached == counts;
}

void test(const string& label, int m, int n, const vector<int>& counts) {
    bool ok = isValid(plantCrops(m, n, counts), counts);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-crop", 2, 2, {4});
    test("wrap-rows", 3, 3, {4, 4, 1});
    test("one-column", 3, 1, {1, 2});
    test("zero-count", 2, 3, {3, 0, 3});
}

// follow-up: two stacked squares, N x N on top of M x M, left-aligned -> same snake over
//   the union's rows (row r has width N for r < N, else M), but pick directions so the
//   last top row (N - 1) runs right->left: it ends at column 0, where the first bottom
//   row starts left->right, so the seam stays adjacent; inside each square widths match.
//   int width = r < N ? N : M;  bool leftToRight = (N - r) % 2 == 0;
// follow-up: counts do not sum to m * n -> reject up front (or leave the extra cells -1)
