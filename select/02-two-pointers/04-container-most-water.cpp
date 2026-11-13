// LC 11 Container With Most Water (Medium) — two pointers, move the shorter side
// pick two lines i < j to maximize (j - i) * min(height[i], height[j])
//
// clarify:
// num of lines -> at least 2
// heights -> >= 0
// line width -> none
// overflow -> assume result fits in int

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// core idea: start widest; a narrower pair w/ the shorter line can't beat current area
// shorter line is done -> move it inward
// T: O(n), S: O(1)
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

// follow-up:
// why not move the taller side -> width shrinks, min() still capped by the shorter line
// trapped water (LC 42) -> same pointers w/ running max per side; lower side adds max - h
