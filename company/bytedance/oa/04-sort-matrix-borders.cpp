// ByteDance OA: peel a rectangular integer matrix into concentric borders. For each
// border independently, collect its elements clockwise starting at the border's
// top-left corner, sort them ascending, and write them back along the same path.
// Path: a full border goes top row left->right, right column top->bottom, bottom row
// right->left, left column bottom->top, visiting each cell once; a border that is a
// single row goes left->right; a single column goes top->bottom. Return the matrix.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::sort;

// Cells of the border with corners (top, left) and (bottom, right), clockwise.
vector<pair<int, int>> borderCells(int top, int left, int bottom, int right) {
    vector<pair<int, int>> cells;
    if (top == bottom) {
        for (int c = left; c <= right; c++) {
            cells.push_back({top, c});
        }
        return cells;
    }

    if (left == right) {
        for (int r = top; r <= bottom; r++) {
            cells.push_back({r, left});
        }
        return cells;
    }

    // each side stops one cell early so the corners are visited once
    for (int c = left; c < right; c++) {
        cells.push_back({top, c});
    }
    for (int r = top; r < bottom; r++) {
        cells.push_back({r, right});
    }
    for (int c = right; c > left; c--) {
        cells.push_back({bottom, c});
    }
    for (int r = bottom; r > top; r--) {
        cells.push_back({r, left});
    }
    return cells;
}

// Walk the layers from the outside in; for each, collect, sort, write back along the
// same path. T: O(mn log(mn)), S: O(m + n) for one border.
vector<vector<int>> sortMatrixBorders(vector<vector<int>> matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    int top = 0;
    int left = 0;
    int bottom = m - 1;
    int right = n - 1;
    while (top <= bottom && left <= right) {
        vector<pair<int, int>> cells = borderCells(top, left, bottom, right);
        vector<int> values;
        for (auto [r, c] : cells) {
            values.push_back(matrix[r][c]);
        }
        sort(values.begin(), values.end());
        for (int i = 0; i < (int)cells.size(); i++) {
            auto [r, c] = cells[i];
            matrix[r][c] = values[i];
        }
        top++;
        left++;
        bottom--;
        right--;
    }
    return matrix;
}

void test(const string& label, const vector<vector<int>>& matrix,
          const vector<vector<int>>& want) {
    cout << label << (sortMatrixBorders(matrix) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-cell", {{5}}, {{5}});
    test("two-by-two", {{4, 3}, {1, 2}}, {{1, 2}, {4, 3}});
    test("single-row", {{3, 1, 2}}, {{1, 2, 3}});
    test("odd-square", {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}},
         {{1, 2, 3}, {9, 5, 4}, {8, 7, 6}});
    test("inner-column", {{1, 1, 1}, {1, 9, 1}, {1, 0, 1}, {1, 1, 1}},
         {{1, 1, 1}, {1, 0, 1}, {1, 9, 1}, {1, 1, 1}});
}
