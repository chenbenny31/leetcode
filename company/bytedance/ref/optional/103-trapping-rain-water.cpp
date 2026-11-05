// ByteDance / LC 42: height[i] = elevation of a width-1 bar; return how much rain water
// is trapped between the bars.
// Follow-up: solve it with a monotonic stack.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// Two pointers; the lower side's max bounds its water. T: O(n), S: O(1).
int trap(const vector<int>& height) {
    int lo = 0;
    int hi = (int)height.size() - 1;
    // tallest bar in height[0..lo]
    int leftMax = 0;
    // tallest bar in height[hi..n-1]
    int rightMax = 0;
    int water = 0;
    while (lo < hi) {
        // the shorter side is the limit: the other side has a wall at least as tall
        if (height[lo] < height[hi]) {
            leftMax = max(leftMax, height[lo]);
            water += leftMax - height[lo];
            lo++;
        } else {
            rightMax = max(rightMax, height[hi]);
            water += rightMax - height[hi];
            hi--;
        }
    }
    return water;
}

// follow-up: decreasing stack; a taller bar closes the basin above the popped bottom.
// T: O(n), S: O(n).
int trapStack(const vector<int>& height) {
    // indices of bars with non-increasing heights
    vector<int> stk;
    int water = 0;
    int n = height.size();
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && height[i] > height[stk.back()]) {
            int bottom = stk.back();
            stk.pop_back();
            // no left wall: water spills out
            if (stk.empty()) { break; }
            int left = stk.back();
            int depth = min(height[left], height[i]) - height[bottom];
            water += depth * (i - left - 1);
        }
        stk.push_back(i);
    }
    return water;
}

void test(const string& label, const vector<int>& height, int want) {
    bool ok = trap(height) == want && trapStack(height) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}, 6);
    test("lc-ex2", {4, 2, 0, 3, 2, 5}, 9);
    test("empty", {}, 0);
}

// follow-up: monotonic stack -> see trapStack() above; popped bottom fills
//   (min(height[left], height[i]) - height[bottom]) * width
// follow-up: simplest O(n) -> leftMax[i], rightMax[i] arrays;
//   water += min(leftMax[i], rightMax[i]) - height[i], O(n) space
// follow-up: 2D (LC 407) -> min-heap of boundary cells; pop lowest, water += level - h
//   for lower neighbors, push them with max(h, level), O(RC log RC)
