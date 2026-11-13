// LC 307 Range Sum Query - Mutable (Medium) — Fenwick tree (binary indexed tree)
// update(index, val): nums[index] = val; sumRange(left, right): sum of nums[left..right]
//
// clarify:
// update assigns, does not add -> keep nums to turn it into a delta
// ratio of updates to queries -> both frequent, need O(log n) each
// negative values -> ok
// sum overflow -> assume fits in int

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: tree[i] covers the (i & -i) values ending at pos i
// point update and prefix sum each touch one node per bit of the idx
// T: O(log n) update / sumRange, O(n log n) build, S: O(n)
class NumArray {
    // nums = current values, to turn an assignment into a delta
    vector<int> nums;
    // 1-indexed; tree[i] = sum of the (i & -i) values ending at pos i
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
        // every node whose range contains pos index + 1
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

// follow-up:
// min / max queries -> segment tree (min has no inverse, prefix subtraction fails)
// range updates -> segment tree w/ lazy tags
// cnt smaller after self (LC 315) -> BIT over value ranks; scan from right, query, add
// cnt inversions -> same scan, or merge sort counting cross pairs
// range update + point query -> BIT over diff array: add at left, subtract at right + 1
// 2D version (LC 308) -> BIT of BITs: two nested loops, 4-corner inclusion-exclusion
// immutable version (LC 303) -> plain prefix sums, O(1) per query
