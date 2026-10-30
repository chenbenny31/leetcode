// LC 1911: the alternating sum of an array is (sum at even indices) - (sum at odd
// indices). Return the max alternating sum of any subsequence of nums.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// odd = best sum of a chosen subsequence of odd length (next pick is subtracted),
// even = best of even length (next pick is added). T: O(n), S: O(1).
long long maxAlternatingSum(const vector<int>& nums) {
    long long odd = 0, even = 0;
    for (int x : nums) {
        long long newOdd = max(odd, even + x);
        long long newEven = max(even, odd - x);
        odd = newOdd;
        even = newEven;
    }
    return odd;
}

void test(const string& label, const vector<int>& nums, long long want) {
    cout << label << (maxAlternatingSum(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {3}, 3);
    test("rising", {1, 2, 3}, 3);
    test("dip", {5, 1, 6}, 10);
    test("two-dips", {2, 9, 1, 8}, 16);
}
