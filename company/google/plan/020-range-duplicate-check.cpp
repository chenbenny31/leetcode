// Range Duplicate Check (Google, interview report)
// nums is fixed; query (l, r) inclusive: are nums[l..r] all distinct? O(1) per query
// reported: x3 in the 2026 Q4 list (L4 onsite, 2nd problem of a 45-min round)
//
// clarify:
// l, r -> 0-based, inclusive, l <= r
// l == r -> true
// values -> any int, hash map
// updates between queries -> no; follow-up
// num of queries -> large: O(n) build, O(1) each

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::min;

// core idea: nxt[i] = next idx w/ the same value
// dup in [l, r] iff some i >= l has nxt[i] <= r
// minNext[l] = min nxt[i] over i >= l (suffix min) -> distinct iff minNext[l] > r
// T: build O(n), query O(1), S: O(n)
class DistinctRanges {
    // minNext[l] = smallest j w/ a twin nums[i] == nums[j], l <= i < j; n if none
    vector<int> minNext;

public:
    DistinctRanges(const vector<int>& nums) {
        int n = nums.size();
        minNext.assign(n + 1, n);
        // seen[value] = nearest idx to the right holding value
        unordered_map<int, int> seen;
        for (int i = n - 1; i >= 0; i--) {
            int nxt = n;
            auto it = seen.find(nums[i]);
            if (it != seen.end()) { nxt = it->second; }
            seen[nums[i]] = i;
            minNext[i] = min(nxt, minNext[i + 1]);
        }
    }

    bool allDistinct(int l, int r) {
        return minNext[l] > r;
    }
};

void test(const string& label, const vector<int>& nums,
          const vector<vector<int>>& queries, const vector<bool>& want) {
    DistinctRanges ranges(nums);
    vector<bool> got;
    for (const vector<int>& query : queries) {
        got.push_back(ranges.allDistinct(query[0], query[1]));
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {4, 7, 4, 9, 7, 2}, {{0, 1}, {0, 2}, {1, 3}, {1, 4}, {3, 5}, {5, 5}},
         {true, false, true, false, true, true});
    test("all-same", {5, 5, 5}, {{0, 0}, {0, 1}, {1, 2}}, {true, false, false});
    test("no-dup", {1, 2, 3}, {{0, 2}}, {true});
    test("negative", {-1, 3, -1}, {{0, 2}, {1, 2}}, {false, true});
}

// follow-up:
// brute force to state first -> hash set per query, O(r - l + 1) each
// same idea from the left -> prv[j] = prev idx w/ the same value; prefix max < l
// window view -> leftmost[r] = left edge of the dup-free window ending at r; l >= it
// point updates -> set of idx per value fixes a few nxt; segment tree for the range min
// cnt distinct values in [l, r] -> offline by r; Fenwick w/ +1 at each last occurrence
// 2D grid, row ranges -> same per row
