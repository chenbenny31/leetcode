// LC 1901: in an m x n grid no two adjacent cells are equal and the outside counts as -1.
// Return any cell strictly greater than its four neighbors.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// value at (r, c), -1 outside the grid
int at(const vector<vector<int>>& g, int r, int c) {
    int m = g.size(), n = g[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n) { return -1; }
    return g[r][c];
}

bool isPeak(const vector<vector<int>>& g, int r, int c) {
    int v = g[r][c];
    return v > at(g, r - 1, c) && v > at(g, r + 1, c) && v > at(g, r, c - 1) &&
           v > at(g, r, c + 1);
}

// straightforward: the global maximum is a peak. T: O(mn), S: O(1).
vector<int> findPeak(const vector<vector<int>>& g) {
    int m = g.size(), n = g[0].size(), br = 0, bc = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (g[r][c] > g[br][bc]) {
                br = r;
                bc = c;
            }
        }
    }
    return {br, bc};
}

// improved: binary search on columns. Take the max cell of the middle column; if a side
// neighbor is bigger, a peak exists on that side (climbing from it never crosses back
// over this column), else the cell is a peak. T: O(m log n), S: O(1).
vector<int> findPeakBinary(const vector<vector<int>>& g) {
    int m = g.size(), n = g[0].size();
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int row = 0;
        for (int r = 1; r < m; r++) {
            if (g[r][mid] > g[row][mid]) { row = r; }
        }
        if (at(g, row, mid - 1) > g[row][mid]) {
            hi = mid - 1;
        } else if (at(g, row, mid + 1) > g[row][mid]) {
            lo = mid + 1;
        } else {
            return {row, mid};
        }
    }
    return {-1, -1};
}

void test(const string& label, const vector<vector<int>>& g) {
    vector<int> a = findPeak(g), b = findPeakBinary(g);
    bool ok = isPeak(g, a[0], a[1]) && isPeak(g, b[0], b[1]);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {{5}});
    test("row", {{1, 3, 2, 4}});
    test("column", {{1}, {9}, {2}});
    test("grid", {{1, 4, 3}, {6, 2, 5}, {7, 8, 9}});
}
