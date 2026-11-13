// LC 307 Range Sum Query - Mutable (Medium) — Fenwick tree (binary indexed tree)
// NumArray(nums); update(index, val) sets nums[index] = val; sumRange(left, right)
// returns the sum of nums[left..right], both ends included.
// Clarify: update assigns (does not add); ratio of updates to queries; negative
//   values; overflow of the sums.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: tree[i] covers the i & -i values that end at position i, so a point update
// and a prefix sum each touch one node per bit of the index.
// T: O(log n) update / sumRange, O(n log n) build, S: O(n).
class NumArray {
    // current values, to turn an assignment into a delta
    vector<int> nums;
    // 1-indexed; tree[i] = sum of the (i & -i) values ending at position i
    vector<int> tree;

    // sum of the first count values
    int prefix(int count) {
        int sum = 0;
        for (int i = count; i > 0; i -= i & -i) {
            sum += tree[i];
        }
        return sum;
    }

public:
    NumArray(const vector<int>& values)
        : nums(values.size(), 0), tree(values.size() + 1, 0) {
        for (int i = 0; i < (int)values.size(); i++) {
            update(i, values[i]);
        }
    }

    void update(int index, int val) {
        int delta = val - nums[index];
        nums[index] = val;
        // every node whose range contains position index + 1
        for (int i = index + 1; i < (int)tree.size(); i += i & -i) {
            tree[i] += delta;
        }
    }

    int sumRange(int left, int right) {
        return prefix(right + 1) - prefix(left);
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    NumArray sums({1, 3, 5, 7});
    check(sums.sumRange(0, 3) == 16, "whole-array");
    check(sums.sumRange(1, 2) == 8, "inner-range");
    sums.update(1, -2);
    check(sums.sumRange(0, 1) == -1 && sums.sumRange(1, 3) == 10, "after-update");
    sums.update(3, 0);
    check(sums.sumRange(3, 3) == 0 && sums.sumRange(0, 3) == 4, "last-index");
    return 0;
}

// follow-up (ideas only):
// - min / max queries, or range updates -> segment tree (min has no inverse, so the
//   prefix(right) - prefix(left) trick fails); lazy tags for range updates
// - count of smaller numbers after self (LC 315) -> BIT over value ranks; scan from the
//   right: query prefix(rank - 1), then add 1 at rank
// - count inversions -> same scan, or merge sort counting the cross pairs
// - range update + point query -> BIT over the difference array: add at left, subtract
//   at right + 1; the point value is a prefix sum
// - 2D version (LC 308) -> BIT of BITs: two nested i += i & -i loops, four-corner
//   inclusion-exclusion for a rectangle
// - immutable version (LC 303) -> plain prefix sums, O(1) per query
