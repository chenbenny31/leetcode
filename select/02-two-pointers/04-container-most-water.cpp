// LC 11 Container With Most Water (Medium). Pattern: two pointers, move shorter side.
// Pick two lines height[i], height[j] (i < j); maximize (j - i) * min(height[i],
// height[j]).

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

// follow-up: why moving the taller side is useless -> width shrinks and min() is still
//   capped by the shorter line, so the area can only drop
// follow-up: trapped water between all bars (LC 42) -> same pointers, sum per bar
