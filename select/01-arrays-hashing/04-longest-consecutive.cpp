// LC 128 Longest Consecutive Sequence (Medium) — hash set, count from run heads
// return len of longest run x, x + 1, ... whose values all appear in unsorted nums
//
// clarify:
// O(n) required? -> yes, so no sort
// dups -> count once
// negative values -> ok
// empty nums -> 0

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

// core idea: count up only from a num w/ no num - 1 (run head); each run walked once
// T: O(n), S: O(n) — inner loop starts only at heads, all inner steps sum to n
int longestConsecutive(const vector<int>& nums) {
    // distinct values of nums
    unordered_set<int> present(nums.begin(), nums.end());
    int best = 0;
    for (int num : present) {
        // not a head: its run is counted from a smaller value
        if (present.count(num - 1)) {
            continue;
        }
        int len = 1;
        while (present.count(num + len)) {
            len++;
        }
        best = max(best, len);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (longestConsecutive(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-runs", {10, 3, 11, 1, 2, 12, 13}, 4);
    test("dups", {2, 1, 2, 3}, 3);
    test("negatives", {-1, 1, 0}, 3);
    test("empty", {}, 0);
    return 0;
}

// follow-up:
// O(1) extra space, input may be modified -> sort, scan, skip equal neighbors; O(n log n)
// return the run itself -> keep the head of the best run, output head .. head + best - 1
