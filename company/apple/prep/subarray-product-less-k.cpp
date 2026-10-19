// count consec subarrays with product < k, all vals are positive

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// sliding-window, shrink until prod < k, T: O(n), S: O(1)
int numSubarrayProductLessThanK(const vector<int>& nums, int k) {
    if (k <= 1) { return 0; }
    const int n = nums.size();
    long long prod = 1;
    int count = 0;

    int left = 0;
    for (int right = 0; right < n; right++) {
        prod *= nums[right];
        while (prod >= k) {
            prod /= nums[left];
            left++;
        }
        count += right - left + 1; // subarrays end at right, each left counts 1
    }
    return count;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    int got = numSubarrayProductLessThanK(nums, k);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {2, 3}, 7, 3);
    test("shrink", {2, 5, 3}, 10, 3);
    test("k1", {1, 1}, 1, 0);
}
