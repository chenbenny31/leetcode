// LC 4063: a subarray is valid if its sum is divisible by k, or becomes divisible after
// negating one of its elements (x -> -x). Return the length of the longest valid
// subarray, or 0 if none. (n <= 1000)

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::max;

long long mod(long long v, int k) { return ((v % k) + k) % k; }

// Negating x changes the sum S to S - 2x, so the subarray is valid iff S % k == 0 or
// some element has 2x % k == S % k. Extend each start to the right, keeping the set of
// 2x % k seen so far. T: O(n^2), S: O(n).
int longestValid(const vector<int>& nums, int k) {
    int n = nums.size(), best = 0;
    for (int i = 0; i < n; i++) {
        long long sum = 0;
        // twice[r] present = some element x in nums[i..j] has 2x % k == r
        unordered_set<long long> twice;
        for (int j = i; j < n; j++) {
            sum += nums[j];
            twice.insert(mod(2LL * nums[j], k));
            long long s = mod(sum, k);
            if (s == 0 || twice.count(s)) { best = max(best, j - i + 1); }
        }
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (longestValid(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("plain", {3, 1}, 4, 2);
    test("negate", {5, 1}, 4, 2);
    test("none", {1, 2}, 4, 0);
    test("one", {2, 4}, 4, 1);
    test("negative", {-3, 1}, 4, 2);
}
