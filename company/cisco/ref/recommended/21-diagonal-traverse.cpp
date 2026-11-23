// 21 · Diagonal Traverse (Cisco LC tag + OA "zig-zag", Medium) — LC 498
// return all cells of an m x n matrix in zig-zag diagonal order: up-right on the first
// diagonal, down-left on the next, alternating
//
// clarify:
// first move -> up-right (LC 498); the OA version may start down-left: swap the parity
// empty matrix -> not given (m, n >= 1)
// output -> one flat list

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;

// core idea: cells with the same r + c = d form one diagonal; walk d = 0..m+n-2, rows
// descending on even d (up-right), ascending on odd d (down-left)
// T: O(m * n), S: O(1) besides the output
vector<int> findDiagonalOrder(const vector<vector<int>>& mat) {
    int m = mat.size();
    int n = mat[0].size();
    vector<int> out;
    for (int d = 0; d <= m + n - 2; d++) {
        // valid rows of diagonal d: c = d - r must stay in 0..n-1
        int lo = max(0, d - n + 1);
        int hi = min(d, m - 1);
        if (d % 2 == 0) {
            for (int r = hi; r >= lo; r--) { out.push_back(mat[r][d - r]); }
        } else {
            for (int r = lo; r <= hi; r++) { out.push_back(mat[r][d - r]); }
        }
    }
    return out;
}

void test(const string& label, const vector<vector<int>>& mat, const vector<int>& want) {
    cout << label << (findDiagonalOrder(mat) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("3x3", {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, {1, 2, 4, 7, 5, 3, 6, 8, 9});
    test("2x3", {{1, 2, 3}, {4, 5, 6}}, {1, 2, 4, 5, 3, 6});
    test("one-row", {{1, 2, 3}}, {1, 2, 3});
    test("one-col", {{1}, {2}}, {1, 2});
    return 0;
}

// follow-up:
// 1. no zig-zag, every diagonal top-down (LC 1424 style) -> drop the parity branch
// 2. jagged rows (LC 1424) -> bucket (r + c) -> values, push rows bottom-up, read buckets
// 3. anti-diagonals only -> same d loop, one direction
// 4. spiral instead -> select/16-bit-math-matrix/01-spiral-matrix
