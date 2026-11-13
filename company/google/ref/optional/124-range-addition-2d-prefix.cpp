// 124 · Range Addition + Range Sum Query 2D — LC 370, 304
// (1) LC 370: zero array of `length`; each [start, end, inc] adds inc; return final array
// (2) LC 304: immutable matrix, many sumRegion(r1, c1, r2, c2) queries
//
// clarify:
// (1) ends -> both inclusive
// (2) corners -> inclusive
// (2) empty matrix -> no queries
// sums -> fit in int

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// (1) range addition
// core idea: diff[start] += inc, diff[end + 1] -= inc (skip past last idx); prefix sum
// T: O(n + q), S: O(n)
vector<int> getModifiedArray(int length, const vector<vector<int>>& updates) {
    // diff[i] = value[i] - value[i - 1]
    vector<int> diff(length, 0);
    for (const vector<int>& update : updates) {
        diff[update[0]] += update[2];
        if (update[1] + 1 < length) { diff[update[1] + 1] -= update[2]; }
    }
    for (int i = 1; i < length; i++) { diff[i] += diff[i - 1]; }
    return diff;
}

// (2) range sum query 2D
// core idea: prefix sums w/ a zero row and col of padding
// region = big corner block - two strips + doubly subtracted corner
// T: build O(m * n), query O(1), S: O(m * n)
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

// follow-up:
// range adds on a grid -> 2D diff: +inc at (r1, c1), (r2 + 1, c2 + 1); -inc at other two
// mutable matrix (LC 308) -> 2D Fenwick tree, O(log m * log n) per update / query
