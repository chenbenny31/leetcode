// LC 42 Trapping Rain Water (Hard). Pattern: two pointers with running side maxes.
// height[i] = elevation of a width-1 bar; return how much rain water is trapped
// between the bars.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Water at i = min(tallest left, tallest right) - height[i]; the lower side's max
// is that min, so settle the lower side and move it inward. T: O(n), S: O(1).
int trap(const vector<int>& height) {
    int left = 0;
    int right = (int)height.size() - 1;
    // tallest bar in height[0..left]
    int leftMax = 0;
    // tallest bar in height[right..n-1]
    int rightMax = 0;
    int water = 0;
    while (left < right) {
        // the shorter side is the limit: the other side has a wall at least as tall
        if (height[left] < height[right]) {
            leftMax = max(leftMax, height[left]);
            water += leftMax - height[left];
            left++;
        } else {
            rightMax = max(rightMax, height[right]);
            water += rightMax - height[right];
            right--;
        }
    }
    return water;
}

void test(const string& label, const vector<int>& height, int want) {
    cout << label << (trap(height) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-basin", {2, 0, 2}, 2);
    test("uneven-walls", {3, 0, 1, 0, 2}, 5);
    test("no-basin", {1, 2, 3}, 0);
    test("empty", {}, 0);
}

// follow-up: easiest to derive -> leftMax[i], rightMax[i] arrays first;
//   water += min(leftMax[i], rightMax[i]) - height[i], O(n) space
// follow-up: monotonic stack -> decreasing stack of indices; a taller bar pops bottom,
//   water += (min(height[left], height[i]) - height[bottom]) * (i - left - 1)
// follow-up: 2D (LC 407) -> min-heap of boundary cells, flood inward, O(mn log mn)
