// LC 1004 Max Consecutive Ones III: longest run of 1s in binary nums after flipping at
// most k zeros.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Longest window holding at most k zeros; shrink from the left when it holds k + 1.
// T: O(n), S: O(1).
int longestOnes(const vector<int>& nums, int k) {
    int n = nums.size();
    int zeros = 0;   // zeros inside the window
    int best = 0;
    int left = 0;

    for (int right = 0; right < n; right++) {
        if (nums[right] == 0) { zeros++; }
        while (zeros > k) {
            if (nums[left] == 0) { zeros--; }
            left++;
        }

        best = max(best, right - left + 1);
    }

    return best;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (longestOnes(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("flip-one", {1, 0, 1, 1, 0, 1}, 1, 4);
    test("no-flips", {1, 1, 0, 1}, 0, 2);
    test("all-zeros", {0, 0, 0}, 2, 2);
    test("empty", {}, 3, 0);
}
