// 92 · Container With Most Water — LC 11
// Mother: select/02-two-pointers/04-container-most-water.cpp
// pick two lines i < j; maximize (j - i) * min(height[i], height[j])
//
// clarify:
// num of lines -> at least 2
// height, n ranges -> area fits in int
// equal heights -> move either side

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// core idea: start w/ the widest pair, move the shorter side inward
// any narrower pair using the shorter line is capped by it -> cannot beat current area
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
}

// follow-up:
// why not move the taller side -> width shrinks, min() still capped by the shorter line
// trapping rain water (LC 42) -> same two pointers, sum water per bar
