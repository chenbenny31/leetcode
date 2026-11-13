// Connected Crop Garden (Google, interview report): an m x n field and crop types
// 0..k-1 with counts[crop] cells each (counts >= 0 and they sum to m * n). Assign a crop
// to every cell so that the cells of each crop form one 4-connected region. Return the
// grid of crop ids; any valid grid is accepted (a crop with count 0 is simply absent).
//
// Clarify:
// - counts sum to m * n -> given (else reject up front: follow-up)
// - any valid grid is accepted -> a construction, no search; count 0 -> crop absent
// - connected = 4-directional; a crop with one cell is connected by itself
// - no shape rule (regions need not be rectangles)
// - m, n up to ~1e3 -> an O(mn) fill
// Idea: walk the field in snake order (row 0 left->right, row 1 right->left, ...).
//   Consecutive snake cells are always 4-adjacent, so the field is one long path, and
//   any contiguous run of a path is connected: crop 0 takes the first counts[0] cells,
//   crop 1 the next counts[1], and so on.
// Pitfalls:
// - plain row-major order fails: the end of row r is not next to the start of row
//   r + 1, so a crop that wraps a row splits (n = 3, counts {2, 2, ...}: crop 1 gets
//   (0, 2) and (1, 0))
// - odd rows map i to column n - 1 - i, not n - i
// - counts must sum to m * n, or seq[pos] reads past the end
// Tests to write: one crop fills the field; crops that wrap row ends; one column; a
//   crop with count 0; stacked: a crop across the seam with an odd top side, wide top.

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

// follow-up (reported): an N x N square (N = top) on top of an M x M square
// (M = bottom), left-aligned; rows 0..N-1 have width N, the next M rows width M.
// Same snake, but the directions are picked so the last top row N - 1 runs
// right->left: it ends at column 0, right above where row N starts left->right, so the
// seam stays adjacent. Inside each square the widths match, as before.
// T: O(N^2 + M^2), S: O(N^2 + M^2).
// Pitfall: a plain snake (row 0 always left->right) breaks the seam when N is odd and
// M != N: row N - 1 then ends at column N - 1 but row N starts at column M - 1.
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

// test helper: each crop has counts[crop] cells and one BFS from its first cell reaches
// all of them (a second unvisited cell of the same crop means a second region). Rows
// may differ in width (stacked squares, left-aligned).
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

// follow-up: two stacked squares, N x N on top of M x M, left-aligned (reported) ->
//   plantStackedSquares above
// follow-up: counts do not sum to m * n -> reject up front:
//   int total = 0;
//   for (int count : counts) { total += count; }
//   if (total != m * n) { return {}; }
