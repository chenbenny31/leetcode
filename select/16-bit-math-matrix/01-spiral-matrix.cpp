// LC 54 Spiral Matrix (Medium). Pattern: four shrinking bounds.
// Return all elements of a non-empty m x n matrix in clockwise spiral order, starting
// at the top-left.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Bounds of the rectangle not yet visited; peel one ring per lap.
// T: O(m * n), S: O(1) besides the output.
vector<int> spiralOrder(const vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    int top = 0;
    int bottom = m - 1;
    int left = 0;
    int right = n - 1;
    vector<int> result;
    while (top <= bottom && left <= right) {
        for (int c = left; c <= right; c++) {
            result.push_back(matrix[top][c]);
        }
        top++;

        for (int r = top; r <= bottom; r++) {
            result.push_back(matrix[r][right]);
        }
        right--;

        // a single remaining row was already read left to right
        if (top <= bottom) {
            for (int c = right; c >= left; c--) {
                result.push_back(matrix[bottom][c]);
            }
            bottom--;
        }

        // a single remaining column was already read top to bottom
        if (left <= right) {
            for (int r = bottom; r >= top; r--) {
                result.push_back(matrix[r][left]);
            }
            left++;
        }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& matrix,
          const vector<int>& want) {
    cout << label << (spiralOrder(matrix) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-row", {{1, 2, 3}}, {1, 2, 3});
    test("one-col", {{1}, {2}, {3}}, {1, 2, 3});
    test("tall", {{1, 2}, {3, 4}, {5, 6}, {7, 8}}, {1, 2, 4, 6, 8, 7, 5, 3});
    test("wide-center-row", {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}},
         {1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7});
}

// follow-up: spiral matrix II (LC 59) -> same four bounds, write val++ instead of read
// follow-up: direction-array version -> dr/dc arrays + visited, turn right when the next
//   cell is out of range or visited; O(m * n) extra space
