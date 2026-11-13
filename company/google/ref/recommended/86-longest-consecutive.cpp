// 86 · Longest Consecutive Sequence — LC 128
// Mother: select/01-arrays-hashing/04-longest-consecutive.cpp
// return len of longest run of consecutive ints (x, x+1, ...) present in unsorted nums
//
// clarify:
// dup -> count once
// negatives -> allowed
// empty input -> 0
// O(n) required -> no sort

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

// core idea: values in a hash set; count up only from a run head (num w/ no num - 1)
// T: O(n), S: O(n) — each run is walked once in total
int longestConsecutive(const vector<int>& nums) {
    // distinct values of nums
    unordered_set<int> present(nums.begin(), nums.end());
    int best = 0;
    for (int num : present) {
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

// follow-up:
// why O(n) w/ a nested loop -> while loop runs only from heads, inner steps sum to n
// O(1) extra space, may modify -> sort + scan skipping equal neighbors, O(n log n)
