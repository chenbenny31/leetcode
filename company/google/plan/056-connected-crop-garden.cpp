// Connected Crop Garden (Google, interview report)
// (1) m x n field, counts[crop] cells per crop (sum = m * n); each crop 4-connected
// (2) same on an N x N square stacked on an M x M square, left-aligned
//
// clarify:
// counts sum to num of cells -> assumed; else reject
// several valid grids -> any is accepted
// count 0 -> crop absent
// 4-directional -> diagonals don't connect
// regions need not be rectangles -> snake runs are fine

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

// (1) rectangle
// core idea: snake order (row 0 left->right, row 1 right->left, ...); steps stay adjacent
// any contiguous run of the snake is connected -> crop k takes the next counts[k] cells
// T: O(mn), S: O(mn)
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

// (2) stacked squares
// core idea: same snake over top rows (width N), then bottom rows (width M)
// last top row must run right->left: it ends at col 0, right above the first bottom cell
// T: O(N^2 + M^2), S: O(N^2 + M^2)
vector<vector<int>> plantStackedSquares(int top, int bottom, const vector<int>& counts) {
    // seq[pos] = crop of the pos-th cell along the snake
    vector<int> seq;
    for (int crop = 0; crop < (int)counts.size(); crop++) {
        seq.insert(seq.end(), counts[crop], crop);
    }
    vector<vector<int>> grid;
    int pos = 0;
    for (int r = 0; r < top + bottom; r++) {
        int width = r < top ? top : bottom;
        // row top - 1 runs right->left, row top left->right, alternating from there
        bool leftToRight = (r + top) % 2 == 0;
        vector<int> row(width);
        for (int i = 0; i < width; i++) {
            int c = leftToRight ? i : width - 1 - i;
            row[c] = seq[pos];
            pos++;
        }
        grid.push_back(row);
    }
    return grid;
}

// test helper: every crop has counts[crop] cells and one BFS reaches all of them
bool isValid(const vector<vector<int>>& grid, const vector<int>& counts) {
    int m = grid.size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // reached[crop] = cells of crop reached by its BFS
    vector<int> reached(counts.size(), 0);
    vector<vector<bool>> seen;
    for (const vector<int>& row : grid) {
        seen.push_back(vector<bool>(row.size(), false));
    }
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < (int)grid[r].size(); c++) {
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
                    if (nr < 0 || nr >= m || nc < 0 || nc >= (int)grid[nr].size()) {
                        continue;
                    }
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

void testStacked(const string& label, int top, int bottom, const vector<int>& counts) {
    bool ok = isValid(plantStackedSquares(top, bottom, counts), counts);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-crop", 2, 2, {4});
    test("wrap-rows", 3, 3, {4, 4, 1});
    test("one-column", 3, 1, {1, 2});
    test("zero-count", 2, 3, {3, 0, 3});
    testStacked("stacked-odd-top-seam", 3, 2, {8, 2, 3});
    testStacked("stacked-even-top", 2, 3, {3, 4, 6});
    testStacked("stacked-wide-top", 3, 1, {5, 5});
}

// follow-up:
// counts don't sum to num of cells -> add them up first and reject (return {})
