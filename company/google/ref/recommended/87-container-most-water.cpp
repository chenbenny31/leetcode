// 87 · Container With Most Water — LC 11
// Mother problem: select/02-two-pointers/04-container-most-water.cpp
//
// LC 11 (Medium), two pointers, move the shorter side: pick two lines height[i],
// height[j] (i < j); maximize (j - i) * min(height[i], height[j]).
//
// Clarify
// - at least two lines → assume; heights up to 1e4, n up to 1e5 → area fits in int
//
// Core idea
// - Start with the widest pair; move the shorter side inward: any narrower pair using
//   the shorter line is capped by it, so it can never beat the current area.
// - T O(n), S O(1).
//
// Tests: tall ends, inner pair wins, two lines
//
// Follow-ups
// - why not move the taller side → width shrinks and min() is still capped by the
//   shorter line, so the area can only drop
// - trapping rain water (LC 42) → same two pointers, sum water per bar
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// Start widest; any narrower pair using the shorter line is capped by it, so it can
// never beat the current area: discard the shorter line. T: O(n), S: O(1).
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
}
