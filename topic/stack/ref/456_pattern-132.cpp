// LC 456 132 Pattern: is there i < j < k with nums[i] < nums[k] < nums[j]?
#include <iostream>
#include <vector>
#include <string>
#include <climits>
using std::cout;
using std::string;
using std::vector;

// Scan right to left. The stack holds candidates for the "3"; every value it pops is a
// valid "2" (smaller than a "3" to its left), and we keep the largest one as third.
// Any element below third is a valid "1". T: O(n), S: O(n).
bool find132pattern(const vector<int>& nums) {
    int n = nums.size();
    vector<int> stk;               // candidate "3"s, decreasing from bottom to top
    long long third = LLONG_MIN;   // largest "2" that has a bigger "3" before it
    for (int i = n - 1; i >= 0; i--) {
        if (nums[i] < third) { return true; }
        while (!stk.empty() && stk.back() < nums[i]) {
            third = stk.back();
            stk.pop_back();
        }
        stk.push_back(nums[i]);
    }
    return false;
}

void test(const string& label, const vector<int>& nums, bool want) {
    if (find132pattern(nums) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("basic", {1, 3, 2}, true);
    test("low-before-peak", {3, 5, 0, 4}, true);
    test("increasing", {1, 2, 3, 4}, false);
    test("equal-values", {2, 2, 2}, false);
}
