// LC 240: every row and every column of matrix is sorted ascending. Return whether target
// is in it.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// straightforward: check every cell. T: O(mn), S: O(1).
bool searchMatrix(const vector<vector<int>>& matrix, int target) {
    for (const auto& row : matrix) {
        for (int x : row) {
            if (x == target) { return true; }
        }
    }
    return false;
}

// improved: start at the top-right corner; a bigger value rules out its column, a smaller
// one rules out its row. T: O(m + n), S: O(1).
bool searchMatrixStair(const vector<vector<int>>& matrix, int target) {
    int m = matrix.size(), n = matrix[0].size();
    int r = 0, c = n - 1;
    while (r < m && c >= 0) {
        if (matrix[r][c] == target) { return true; }
        if (matrix[r][c] > target) {
            c--;
        } else {
            r++;
        }
    }
    return false;
}

void test(const string& label, const vector<vector<int>>& matrix, int target, bool want) {
    bool ok = searchMatrix(matrix, target) == want &&
              searchMatrixStair(matrix, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<vector<int>> m = {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}};
    test("middle", m, 5, true);
    test("corner", m, 9, true);
    test("missing", m, 10, false);
    test("too-small", m, 0, false);
    test("single", {{3}}, 3, true);
}
