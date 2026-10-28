// return the len of the longest run of consecutive ints in unsorted nums
// required O(n)

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::sort;
using std::max;

// sort, then scan runs, skip duplicates, T: O(n log n), S: O(1)
int longestConsecutive(vector<int> nums) {
    if (nums.empty()) { return 0; }
    sort(nums.begin(), nums.end());
    int best = 1;
    int len = 1; // len of the run ending at nums[i]
    int n = nums.size();
    for (int i = 1; i < n; i++) {
        if (nums[i] == nums[i - 1]) { continue; }
        if (nums[i] == nums[i - 1] + 1) {
            len++;
        } else {
            len = 1;
        }
        best = max(best, len);
    }
    return best;
}

// hash-set, count up only from a run head (no x - 1), T: O(n), S: O(n)
int longestConsecutiveSet(const vector<int>& nums) {
    unordered_set<int> present(nums.begin(), nums.end()); // distinct vals of nums
    int best = 0;
    for (int x : present) {
        // non-heads skip, so each run is walked once in total
        if (present.count(x - 1)) { continue; }
        int len = 1;
        while (present.count(x + len)) { len++; }
        best = max(best, len);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    bool ok = longestConsecutive(nums) == want && longestConsecutiveSet(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {5, 1, 3, 2}, 3);
    test("dup", {1, 2, 2, 3}, 3);
    test("empty", {}, 0);
}
