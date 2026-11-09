// LC 477 Total Hamming Distance (Medium). Pattern: count ones per bit position.
// Return the sum of Hamming distances over all pairs of nums (0 <= nums[i] < 2^31).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// At each bit, every (one, zero) pair differs: add ones * zeros. T: O(31n), S: O(1).
int totalHammingDistance(const vector<int>& nums) {
    int n = nums.size();
    int total = 0;
    for (int bit = 0; bit < 31; bit++) {
        // numbers with this bit set
        int ones = 0;
        for (int num : nums) {
            ones += (num >> bit) & 1;
        }
        total += ones * (n - ones);
    }
    return total;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (totalHammingDistance(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {1, 2, 3}, 4);
    test("high-bit", {0, 1 << 30}, 1);
    test("all-equal", {7, 7, 7}, 0);
    test("single", {5}, 0);
}

// follow-up: brute force -> popcount(a ^ b) for every pair, O(32 * n^2); flipping the
//   loops to per-bit counting removes the pairs
