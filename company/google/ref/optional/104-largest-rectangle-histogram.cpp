// 104 · Largest Rectangle in Histogram — LC 84
// Mother: select/optional/05-stack/04-largest-rectangle-histogram.cpp
// bars of width 1 w/ given heights; return area of largest rectangle
// reported: LC tag #84; maximal rectangle in binary matrix (LC 85) x2 (FastPrep)
//
// clarify:
// heights non-negative, equal allowed -> pop on >=
// n >= 1 -> no empty input
// area overflow? -> assume fits in int

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::max;
using std::string;
using std::vector;

// core idea: stack of idx w/ strictly increasing heights
// on pop: cur bar = first lower on right, new top = first lower on left -> width known
// T: O(n), S: O(n)
int largestRectangleArea(vector<int> heights) {
    // sentinel 0 flushes every bar left on stack
    heights.push_back(0);
    int n = heights.size();
    // idx w/ strictly increasing heights
    vector<int> stk;
    int best = 0;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && heights[stk.back()] >= heights[i]) {
            int height = heights[stk.back()];
            stk.pop_back();
            // first lower bar on left, -1 if none
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

// follow-up:
// maximal rectangle in 0/1 matrix (LC 85) -> per row heights[c] = +1 on '1' else 0; O(mn)
// max score of good subarray (LC 1793) -> same w/ i <= k <= j, or two pointers from k
