// Range Queries: Subarray with Alternating Parity (Google, interview report)
// Given nums (length n) and q queries [l, r] (0-based, inclusive), answer for each query
// whether every adjacent pair in nums[l..r] has different parity (one odd, one even).
// A single-element range is true. Each query must be O(1) after preprocessing.
//
// Clarify:
// - n, q up to 1e5 -> O(n + q); scanning every range is O(n q) = 1e10
// - negatives -> allowed; parity via & 1 (in C++ -3 % 2 == -1, so % 2 misreads them)
// - l == r -> true; queries are 0-based, inclusive, l <= r
// - only the pairs inside [l, r] count: (l - 1, l) and (r, r + 1) do not
// Idea: call pair (i, i + 1) bad when both values have the same parity. [l, r] alternates
//   iff none of the pairs l .. r - 1 is bad, so a prefix count of bad pairs answers each
//   query with one subtraction.
// Pitfalls:
// - the pairs of [l, r] are l .. r - 1: prefixBad[r] - prefixBad[l], not prefixBad[r + 1]
// - n - 1 pairs: the loop runs while i + 1 < n
// - x % 2 is -1 for a negative odd x; compare (x & 1) instead
// Tests to write: fully alternating; one bad pair with queries around it; single
//   elements; negatives.

#include <iostream>
#include <vector>
#include <string>
#include <set>

using std::cout;
using std::string;
using std::vector;
using std::set;

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

// follow-up: point updates nums[i] = value between queries, and firstBreak(l, r) = the
// first i in [l, r - 1] whose pair (i, i + 1) is bad, -1 if nums[l..r] alternates. Keep
// the bad pair indices in an ordered set; a change at i can only flip pairs i - 1 and i.
// Pitfall: erase both pairs around i before writing the value, then re-add the bad ones;
// skip pair i - 1 at i == 0 and pair i at i == n - 1.
// T: build O(n log n), update / firstBreak O(log n); S: O(n).
class ParityRanges {
    // current values
    vector<int> nums;
    // indices i where nums[i] and nums[i + 1] have the same parity
    set<int> badAt;

    // stores pair (i, i + 1) in badAt if it exists and is bad
    void addIfBad(int i) {
        if (i < 0 || i + 1 >= (int)nums.size()) { return; }
        if ((nums[i] & 1) == (nums[i + 1] & 1)) { badAt.insert(i); }
    }

public:
    ParityRanges(const vector<int>& values) : nums(values) {
        for (int i = 0; i + 1 < (int)nums.size(); i++) { addIfBad(i); }
    }

    void update(int i, int value) {
        badAt.erase(i - 1);
        badAt.erase(i);
        nums[i] = value;
        addIfBad(i - 1);
        addIfBad(i);
    }

    // first bad pair inside [l, r], -1 if none; nums[l..r] alternates iff this is -1
    int firstBreak(int l, int r) {
        auto it = badAt.lower_bound(l);
        if (it == badAt.end() || *it >= r) { return -1; }
        return *it;
    }
};

void test(const string& label, const vector<int>& nums,
          const vector<vector<int>>& queries, const vector<bool>& want) {
    bool ok = alternatingParity(nums, queries) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

// applies every [i, value] update, then compares firstBreak for each [l, r] query
void testDynamic(const string& label, const vector<int>& nums,
                 const vector<vector<int>>& updates, const vector<vector<int>>& queries,
                 const vector<int>& want) {
    ParityRanges ranges(nums);
    for (const vector<int>& update : updates) { ranges.update(update[0], update[1]); }
    vector<int> got;
    for (const vector<int>& query : queries) {
        got.push_back(ranges.firstBreak(query[0], query[1]));
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("alternating", {1, 2, 3, 4}, {{0, 3}}, {true});
    test("even-pair", {1, 2, 4, 5}, {{0, 3}, {0, 1}, {1, 2}, {2, 3}},
         {false, true, false, true});
    test("single", {2, 2}, {{0, 0}, {1, 1}, {0, 1}}, {true, true, false});
    test("negative", {-3, 2, -1}, {{0, 2}}, {true});

    testDynamic("dyn-no-updates", {1, 2, 4, 5}, {}, {{0, 3}, {0, 1}, {2, 3}},
                {1, -1, -1});
    testDynamic("dyn-move-break", {1, 2, 4, 5}, {{2, 3}}, {{0, 3}, {0, 2}}, {2, -1});
    testDynamic("dyn-new-break", {1, 2, 3}, {{0, 2}}, {{0, 2}, {1, 2}}, {0, -1});
    testDynamic("dyn-last-index", {1, 2}, {{1, 3}}, {{0, 1}, {1, 1}}, {0, -1});
}

// follow-up: point updates between queries -> ParityRanges above; a Fenwick tree over the
//   bad flags also works (alternates iff the range sum over pairs l .. r - 1 is 0)
// follow-up: report the first break in [l, r] -> ParityRanges::firstBreak above
// follow-up: count all alternating subarrays (LC 3101 shape) -> each end i adds the
//   length of the alternating run ending at i; total is long long (up to n^2 / 2):
//     run = (i > 0 && (nums[i] & 1) != (nums[i - 1] & 1)) ? run + 1 : 1;
//     total += run;
