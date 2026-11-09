// LC 128 Longest Consecutive Sequence (Medium). Pattern: hash set, count from run heads.
// Return the length of the longest run of consecutive integers (x, x+1, ...) whose
// values all appear in unsorted nums, in O(n).

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

// Count up only from num with no num - 1 (the head of a run). T: O(n), S: O(n).
int longestConsecutive(const vector<int>& nums) {
    // distinct values of nums
    unordered_set<int> present(nums.begin(), nums.end());
    int best = 0;
    for (int num : present) {
        // non-heads skip, so each run is walked once in total
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
}

// follow-up: why O(n) -> the while loop runs only from heads, so all inner steps sum to n
// follow-up: O(1) extra space allowed to modify -> sort + scan, skip equal neighbors,
//   O(n log n)
