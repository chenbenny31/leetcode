// LC 54 Spiral Matrix: return all elements of an m x n matrix in spiral order,
// starting at the top-left and going clockwise.
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::string;
using std::vector;

// Four shrinking borders; after the top row and right column, re-check that rows and
// columns remain before walking back, or a single row/column is visited twice.
// T: O(m*n), S: O(1) besides the output.
vector<int> spiralOrder(vector<vector<int>>& matrix) {
    int top = 0;
    int bottom = matrix.size() - 1;
    int left = 0;
    int right = matrix[0].size() - 1;
    vector<int> res;
    while (top <= bottom && left <= right) {
        for (int c = left; c <= right; c++) { res.push_back(matrix[top][c]); }
        top++;
        for (int r = top; r <= bottom; r++) { res.push_back(matrix[r][right]); }
        right--;
        if (top <= bottom) {
            for (int c = right; c >= left; c--) { res.push_back(matrix[bottom][c]); }
            bottom--;
        }
        if (left <= right) {
            for (int r = bottom; r >= top; r--) { res.push_back(matrix[r][left]); }
            left++;
        }
    }
    return res;
}

void test(const string& label, vector<vector<int>> matrix, const vector<int>& want) {
    cout << label << (spiralOrder(matrix) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("square", {{1, 2}, {3, 4}}, {1, 2, 4, 3});
    test("wide", {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}},
         {1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7});
    test("one-row", {{1, 2, 3}}, {1, 2, 3});
    test("one-column", {{1}, {2}, {3}}, {1, 2, 3});
}
