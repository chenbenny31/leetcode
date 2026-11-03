// LC 54: return all elements of an m x n matrix in clockwise spiral order.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Walk one ring at a time, then shrink all four bounds. Every loop also stops once all
// m*n cells are taken, so a lone middle row/column is never walked back.
// T: O(m*n), S: O(1) extra.
vector<int> spiralOrder(const vector<vector<int>>& matrix) {
    int m = matrix.size(), n = matrix[0].size();
    int total = m * n;
    vector<int> result;
    // current ring: rows [up, down], columns [left, right]
    int up = 0, down = m - 1, left = 0, right = n - 1;
    while ((int)result.size() < total) {
        for (int c = left; c <= right && (int)result.size() < total; c++) {
            result.push_back(matrix[up][c]);
        }
        for (int r = up + 1; r <= down && (int)result.size() < total; r++) {
            result.push_back(matrix[r][right]);
        }
        for (int c = right - 1; c >= left && (int)result.size() < total; c--) {
            result.push_back(matrix[down][c]);
        }
        for (int r = down - 1; r > up && (int)result.size() < total; r--) {
            result.push_back(matrix[r][left]);
        }
        up++;
        down--;
        left++;
        right--;
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& matrix,
          const vector<int>& want) {
    cout << label << (spiralOrder(matrix) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("square", {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, {1, 2, 3, 6, 9, 8, 7, 4, 5});
    test("wide", {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}},
         {1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7});
    test("row", {{1, 2, 3}}, {1, 2, 3});
    test("col", {{1}, {2}, {3}}, {1, 2, 3});
}
