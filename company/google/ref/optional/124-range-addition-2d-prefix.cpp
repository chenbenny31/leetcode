// Two prefix-sum parts.
// (1) LC 370 Range Addition: an all-zero array of `length`; updates [start, end, inc]
//     add inc to every index in [start, end] inclusive. Return the final array.
// (2) LC 304 Range Sum Query 2D: immutable matrix, many sumRegion(r1, c1, r2, c2)
//     queries with inclusive corners.
//
// Clarify:
// - (1) indexes inclusive on both ends; (2) empty matrix -> guard rows == 0
// - sums fit in int for LC limits
// Idea (1): difference array: diff[start] += inc, diff[end + 1] -= inc (if end + 1 <
//   length), then prefix-sum.
// Idea (2): prefix[r+1][c+1] = m[r][c] + prefix[r][c+1] + prefix[r+1][c] - prefix[r][c]
//   with a zero row / column of padding;
//   sumRegion = prefix[r2+1][c2+1] - prefix[r1][c2+1] - prefix[r2+1][c1] + prefix[r1][c1]
// Pitfalls:
// - (1) skip the end + 1 write at the last index
// - (2) off-by-one: padded indexes are +1; add back the doubly subtracted corner
// Tests to write: (1) overlapping updates; update at the last index.
//   (2) single cell; whole matrix; 2 x 2 corner block.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// (1) difference array, then prefix-sum it in place. T: O(n + q), S: O(n).
vector<int> getModifiedArray(int length, const vector<vector<int>>& updates) {
    // diff[i] = value[i] - value[i - 1]
    vector<int> diff(length, 0);
    for (const vector<int>& update : updates) {
        diff[update[0]] += update[2];
        if (update[1] + 1 < length) { diff[update[1] + 1] -= update[2]; }
    }
    // the prefix sum of diff is the final array
    for (int i = 1; i < length; i++) { diff[i] += diff[i - 1]; }
    return diff;
}

// (2) padded 2D prefix sums. T: O(m * n) build, O(1) query, S: O(m * n).
class NumMatrix {
    // prefix[r][c] = sum of the block with corners (0, 0) and (r - 1, c - 1)
    vector<vector<int>> prefix;

public:
    NumMatrix(const vector<vector<int>>& matrix) {
        int m = matrix.size();
        int n = m == 0 ? 0 : matrix[0].size();
        prefix.assign(m + 1, vector<int>(n + 1, 0));
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                prefix[r + 1][c + 1] = matrix[r][c] + prefix[r][c + 1]
                                     + prefix[r + 1][c] - prefix[r][c];
            }
        }
    }

    int sumRegion(int r1, int c1, int r2, int c2) {
        return prefix[r2 + 1][c2 + 1] - prefix[r1][c2 + 1] - prefix[r2 + 1][c1]
             + prefix[r1][c1];
    }
};

void testRange(const string& label, int length, const vector<vector<int>>& updates,
               const vector<int>& want) {
    bool ok = getModifiedArray(length, updates) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testRegion(const string& label, const vector<vector<int>>& matrix, int r1, int c1,
                int r2, int c2, int want) {
    bool ok = NumMatrix(matrix).sumRegion(r1, c1, r2, c2) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    testRange("overlap", 5, {{1, 3, 2}, {2, 4, 3}}, {0, 2, 5, 5, 3});
    testRange("last-index", 3, {{2, 2, 5}}, {0, 0, 5});

    vector<vector<int>> grid = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    testRegion("single-cell", grid, 1, 1, 1, 1, 5);
    testRegion("whole", grid, 0, 0, 2, 2, 45);
    testRegion("corner-block", grid, 1, 1, 2, 2, 28);
}

// follow-up: range adds on a grid -> 2D difference: +inc at (r1, c1), -inc at
//   (r1, c2+1) and (r2+1, c1), +inc at (r2+1, c2+1), then 2D prefix sums
// follow-up: mutable matrix (LC 308) -> 2D Fenwick tree, O(log m * log n) update/query
