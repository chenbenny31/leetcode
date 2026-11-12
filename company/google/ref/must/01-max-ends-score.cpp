// The score of a subarray nums[i..j] (i < j) is nums[i] + nums[j] minus the sum of the
// elements strictly between them. Return the maximum score over all such subarrays.
//
// Clarify:
// - n >= 2? -> assume so (i < j needs two elements); n up to 1e5 -> O(n); the O(n^2)
//   double loop is the first answer only
// - value range -> assume |nums[i]| <= 1e4, so every score fits in int; values up to 1e9
//   -> long long for best / left / middle
// - i and j adjacent -> allowed: empty middle, score nums[i] + nums[j]
// - negatives -> yes; a negative middle raises the score, so a long span can win
// - return the score only (the indices are a follow-up)
// Idea: left = best (nums[i] - middle) over starts i, for subarrays ending just before j.
//   Moving past j, nums[j] either starts a new subarray or becomes one more middle
//   element (left - nums[j]). Every start gets the same future updates, so only the best
//   one is kept, as in Kadane. One pass, O(1) space.
// Pitfalls:
// - score j with the old left, then update left (j cannot be the start and the end)
// - best starts at INT_MIN, not 0: every score can be negative
// - double loop: add nums[j] to middle after scoring (i, j), not before
// - fast loop: left = nums[0] and j starts at 1
// Tests to write: two elements; a later restart wins; a negative middle helps; a long
//   negative middle; all negative.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::max;

// straightforward: fix the first index, extend the last one, keep the middle sum.
// T: O(n^2), S: O(1).
int maxEndsScore(const vector<int>& nums) {
    int n = nums.size();
    int best = INT_MIN;
    for (int i = 0; i < n; i++) {
        // sum of nums[i+1..j-1]
        int middle = 0;
        for (int j = i + 1; j < n; j++) {
            best = max(best, nums[i] + nums[j] - middle);
            middle += nums[j];
        }
    }
    return best;
}

// improved: left = best (first - middle) over subarrays that end right before j.
// nums[j] either starts a new one, or extends the old one and becomes a middle.
// Two starts get the same future updates, so only the best one matters.
// T: O(n), S: O(1).
int maxEndsScoreFast(const vector<int>& nums) {
    int n = nums.size();
    int best = INT_MIN;
    int left = nums[0];
    for (int j = 1; j < n; j++) {
        best = max(best, left + nums[j]);
        left = max(nums[j], left - nums[j]);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    bool ok = maxEndsScore(nums) == want && maxEndsScoreFast(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("pair", {4, 5}, 9);
    test("restart", {1, 9, 1, 9}, 17);
    test("neg-mid", {1, -5, 1}, 7);
    test("long-neg-mid", {2, -1, -1, 2}, 6);
    test("all-neg", {-1, -2, -3}, -2);
}

// follow-up: return the indices (i, j) -> remember where left started:
//     if (left + nums[j] > best) { best = left + nums[j]; bestPair = {leftStart, j}; }
//     if (nums[j] >= left - nums[j]) { left = nums[j]; leftStart = j; }
//     else { left -= nums[j]; }
// follow-up: why the O(n) is right / j - i >= d -> prefix form. With prefix[x] = sum of
//   the first x values, score(i, j) = (prefix[i + 1] + nums[i]) + (nums[j] - prefix[j]),
//   so keep the max first term over allowed i (d = 1 is the original problem):
//     for (int j = d; j < n; j++) {
//         bestFirst = max(bestFirst, prefix[j - d + 1] + nums[j - d]);
//         best = max(best, bestFirst + nums[j] - prefix[j]);
//     }
