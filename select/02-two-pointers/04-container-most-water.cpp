// LC 11 Container With Most Water (Medium) — two pointers, move the shorter side
// Pick two lines i < j to maximize (j - i) * min(height[i], height[j]).
// Clarify: at least two lines; heights >= 0; the lines have no width; the result fits
//   in int.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// Core idea: start widest; any narrower pair that uses the shorter line is capped by it,
// so it cannot beat the current area: discard the shorter line. T: O(n), S: O(1).
int maxArea(const vector<int>& height) {
    int left = 0;
    int right = (int)height.size() - 1;
    int best = 0;
    while (left < right) {
        int width = right - left;
        best = max(best, width * min(height[left], height[right]));
        if (height[left] < height[right]) {
            left++;
        } else {
            right--;
        }
    }
    return best;
}

void test(const string& label, const vector<int>& height, int want) {
    cout << label << (maxArea(height) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("tall-ends", {3, 1, 1, 3}, 9);
    test("inner-pair", {1, 5, 4, 3}, 6);
    test("two-lines", {2, 2}, 2);
    return 0;
}

// follow-up (ideas only):
// - why moving the taller side is useless -> the width shrinks and min() is still capped
//   by the shorter line, so the area can only drop
// - trapped water between all bars (LC 42) -> same two pointers with a running max on
//   each side; the lower side adds its max minus its height
