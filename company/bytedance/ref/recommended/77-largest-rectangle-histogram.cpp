// ByteDance (hack2hire) / LC 84: bars of width 1 with the given heights; return the area
// of the largest rectangle inside the histogram.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Monotonic increasing stack of indices; a bar's rectangle ends at the first lower bar.
// T: O(n), S: O(n).
int largestRectangleArea(vector<int> heights) {
    // sentinel: flushes every bar left on the stack
    heights.push_back(0);
    int n = heights.size();
    // indices with strictly increasing heights
    vector<int> stk;
    int best = 0;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && heights[stk.back()] >= heights[i]) {
            int h = heights[stk.back()];
            stk.pop_back();
            // the bar below on the stack is the first lower bar to the left
            int left = stk.empty() ? -1 : stk.back();
            best = max(best, h * (i - left - 1));
        }
        stk.push_back(i);
    }
    return best;
}

void test(const string& label, const vector<int>& heights, int want) {
    cout << label << (largestRectangleArea(heights) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {2, 1, 5, 6, 2, 3}, 10);
    test("increasing", {2, 4}, 4);
    test("single", {1}, 1);
}

// follow-up: maximal rectangle in 0/1 matrix (LC 85) -> heights[c] per row (+1 on '1',
//   reset to 0 on '0'), run this per row; O(R*C)
// follow-up: explicit bounds -> left[i] = prev smaller, right[i] = next smaller via two
//   stack passes; area = h * (right - left - 1)
// follow-up: sum of subarray minimums (LC 907) -> same prev/next smaller stacks;
//   a[i] * (i - left) * (right - i), strict < on one side to avoid double count
