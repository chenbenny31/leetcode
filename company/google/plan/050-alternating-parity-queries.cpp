// Range Queries: Subarray with Alternating Parity (Google, interview report)
// (1) query [l, r] inclusive: all adjacent pairs in nums[l..r] differ in parity? O(1)
// (2) point updates + first bad pair in [l, r]
//
// clarify:
// n, q size -> O(1) per query after O(n) build
// negatives -> parity via & 1 (-3 % 2 == -1)
// l == r -> true
// only pairs inside [l, r] cnt -> pairs l .. r - 1

#include <iostream>
#include <vector>
#include <string>
#include <set>

using std::cout;
using std::string;
using std::vector;
using std::set;

// (1) static queries
// core idea: pair (i, i + 1) bad = same parity; ok iff prefixBad[r] - prefixBad[l] == 0
// T: O(n + q), S: O(n)
vector<bool> alternatingParity(const vector<int>& nums,
                               const vector<vector<int>>& queries) {
    int n = nums.size();
    // prefixBad[k] = num of bad pairs (i, i + 1) w/ i < k
    vector<int> prefixBad(n, 0);
    for (int i = 0; i + 1 < n; i++) {
        // & 1 = parity bit, also for negatives
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

// (2) point updates + first bad pair
// core idea: ordered set of bad pair idx; a change at i flips only pairs i - 1 and i
// T: build O(n log n), update / firstBreak O(log n), S: O(n)
class ParityRanges {
    // current values
    vector<int> nums;
    // idx i where nums[i] and nums[i + 1] have the same parity
    set<int> badAt;

    // adds pair (i, i + 1) to badAt if it exists and is bad
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

    // first bad pair inside [l, r], -1 if none
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

// applies every [i, value] update, then checks firstBreak for each [l, r]
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

// follow-up:
// point updates, yes / no only -> Fenwick over bad flags; ok iff sum over l .. r - 1 == 0
// cnt all alternating subarrays (LC 3101) -> each end i adds len of the run ending at i
