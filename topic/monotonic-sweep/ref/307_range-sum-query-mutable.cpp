// LC 307 Range Sum Query - Mutable: support update(index, val) and sumRange(left, right).
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Fenwick (binary indexed) tree: a point update and a prefix sum each touch O(log n)
// cells, and a range sum is the difference of two prefix sums.
class NumArray {
    int n;
    vector<int> nums;   // current values
    // tree[i] = sum of the lowbit(i) values ending at 1-based position i
    vector<int> tree;

    // Add delta at index i, climbing by lowbit. T: O(log n), S: O(1).
    void add(int i, int delta) {
        for (i++; i <= n; i += i & -i) { tree[i] += delta; }
    }

    // Sum of nums[0..i-1], descending by lowbit. T: O(log n), S: O(1).
    int prefix(int i) {
        int sum = 0;
        for (; i > 0; i -= i & -i) { sum += tree[i]; }
        return sum;
    }

public:
    // Add the values one by one. T: O(n log n), S: O(n).
    NumArray(const vector<int>& values) : n(values.size()), nums(n, 0), tree(n + 1, 0) {
        for (int i = 0; i < n; i++) { update(i, values[i]); }
    }

    // Add the difference from the old value. T: O(log n), S: O(1).
    void update(int index, int val) {
        add(index, val - nums[index]);
        nums[index] = val;
    }

    // T: O(log n), S: O(1).
    int sumRange(int left, int right) {
        return prefix(right + 1) - prefix(left);
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    NumArray a({4, -1, 3, 2});
    check(a.sumRange(0, 3) == 8 && a.sumRange(1, 2) == 2, "initial");

    a.update(1, 5);
    check(a.sumRange(0, 1) == 9 && a.sumRange(0, 3) == 14, "update");

    a.update(1, 5);
    check(a.sumRange(0, 3) == 14, "same-value-again");
    check(a.sumRange(2, 2) == 3, "single");
}
