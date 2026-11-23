// 14 · Find Last Cell (Cisco OA, Medium) — LC 54 spiral, counter-clockwise
// N x M grid; start at (1, 1), walk the spiral counter-clockwise (down the first column
// first) and land on every other cell (1st, 3rd, 5th, ...); return the last landing cell
//
// clarify:
// statement known only from summaries -> confirm the direction (down first) and that
//   "every other cell" means spiral positions 0, 2, 4, ...
// output -> 1-based (row, col)
// 1 x 1 -> (1, 1)

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// core idea: build the counter-clockwise spiral with four shrinking bounds (down the left
// column, right along the bottom, up the right column, left along the top); the answer
// is the last even position
// T: O(N * M), S: O(N * M)
pair<int, int> findLastCell(int rows, int cols) {
    // cells in spiral order, 0-based
    vector<pair<int, int>> order;
    int top = 0;
    int bottom = rows - 1;
    int left = 0;
    int right = cols - 1;
    while (top <= bottom && left <= right) {
        for (int r = top; r <= bottom; r++) { order.push_back({r, left}); }
        left++;
        if (left <= right) {
            for (int c = left; c <= right; c++) { order.push_back({bottom, c}); }
            bottom--;
        }
        if (top <= bottom && left <= right) {
            for (int r = bottom; r >= top; r--) { order.push_back({r, right}); }
            right--;
        }
        if (top <= bottom && left <= right) {
            for (int c = right; c >= left; c--) { order.push_back({top, c}); }
            top++;
        }
    }
    int last = (int)order.size() - 1;
    // landings are the even positions
    if (last % 2 == 1) { last--; }
    return {order[last].first + 1, order[last].second + 1};
}

void test(const string& label, int rows, int cols, pair<int, int> want) {
    cout << label << (findLastCell(rows, cols) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one", 1, 1, {1, 1});
    test("2x2", 2, 2, {2, 2});
    test("3x3-center", 3, 3, {2, 2});
    test("2x3", 2, 3, {1, 3});
    return 0;
}

// follow-up:
// 1. clockwise (LC 54) -> right along the top first, select/16-bit-math-matrix/01
// 2. O(1) space -> skip whole rings (2 * (h + w) - 4 cells each), walk the last one
// 3. jump every k-th cell -> last landing = position ((N * M - 1) / k) * k
