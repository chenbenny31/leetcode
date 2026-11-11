// LC 84 Largest Rectangle in Histogram (Hard). Pattern: monotonic increasing stack.
// Bars of width 1 with the given heights; return the area of the largest rectangle.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::max;
using std::string;
using std::vector;

// When a bar is popped, the current bar and the new top are its first lower bars on the
// right and left, so its widest rectangle is known. T: O(n), S: O(n).
int largestRectangleArea(vector<int> heights) {
    // sentinel 0 flushes every bar left on the stack
    heights.push_back(0);
    int n = heights.size();
    // indices with strictly increasing heights
    vector<int> stk;
    int best = 0;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && heights[stk.back()] >= heights[i]) {
            int height = heights[stk.back()];
            stk.pop_back();
            // first lower bar on the left, -1 if none
            int left = stk.empty() ? -1 : stk.back();
            int width = i - left - 1;
            best = max(best, height * width);
        }
        stk.push_back(i);
    }
    return best;
}

void test(const string& label, const vector<int>& heights, int want) {
    cout << label << (largestRectangleArea(heights) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("plateau", {1, 3, 3, 1}, 6);
    test("increasing", {1, 2, 3}, 4);
    test("low-and-wide", {2, 1, 2, 1, 2}, 5);
    test("single", {5}, 5);
}

// follow-up: maximal rectangle in a 0/1 matrix (LC 85) -> per row, heights[c] += 1 on '1'
//   else reset to 0, run this on each row, O(m * n)
