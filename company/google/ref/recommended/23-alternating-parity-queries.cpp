// Range Queries: Subarray with Alternating Parity (Google, interview report)
// Given nums (length n) and q queries [l, r] (0-based, inclusive), answer for each query
// whether every adjacent pair in nums[l..r] has different parity (one odd, one even).
// A single-element range is true. Each query must be O(1) after preprocessing.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// count same-parity ("bad") adjacent pairs with a prefix sum; [l, r] covers pairs
// (l, l+1) .. (r-1, r) and is alternating iff none of them is bad. T: O(n + q), S: O(n).
vector<bool> alternatingParity(const vector<int>& nums,
                               const vector<vector<int>>& queries) {
    int n = nums.size();
    // prefixBad[k] = number of bad pairs (i, i+1) with i < k
    vector<int> prefixBad(n, 0);
    for (int i = 0; i + 1 < n; i++) {
        // & 1 gives the parity bit, also for negative numbers
        bool bad = (nums[i] & 1) == (nums[i + 1] & 1);
        prefixBad[i + 1] = prefixBad[i] + bad;
    }
    vector<bool> result;
    for (const vector<int>& query : queries) {
        int l = query[0];
        int r = query[1];
        result.push_back(prefixBad[r] - prefixBad[l] == 0);
    }
    return result;
}

void test(const string& label, const vector<int>& nums,
          const vector<vector<int>>& queries, const vector<bool>& want) {
    bool ok = alternatingParity(nums, queries) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("alternating", {1, 2, 3, 4}, {{0, 3}}, {true});
    test("even-pair", {1, 2, 4, 5}, {{0, 3}, {0, 1}, {1, 2}, {2, 3}},
         {false, true, false, true});
    test("single", {2, 2}, {{0, 0}, {1, 1}, {0, 1}}, {true, true, false});
    test("negative", {-3, 2, -1}, {{0, 2}}, {true});
}

// follow-up: point updates between queries -> keep the bad flags in a Fenwick tree;
//   changing nums[i] flips at most pairs (i-1, i) and (i, i+1). O(log n) per op
// follow-up: report the first break in [l, r] -> keep the sorted indices i of bad pairs
//   and lower_bound(l); it breaks the range iff that index < r
