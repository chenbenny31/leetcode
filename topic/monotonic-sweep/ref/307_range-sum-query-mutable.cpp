// LC 307 Range Sum Query - Mutable: NumArray(nums); update(index, val) sets
// nums[index] = val; sumRange(left, right) returns the sum of nums[left..right].
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

class NumArray {
    int n;
    vector<int> nums;   // current values
    // Fenwick tree: tree[i] = sum of the lowbit(i) values ending at 1-based position i
    vector<int> tree;

    void add(int i, int delta) {
        for (i++; i <= n; i += i & -i) { tree[i] += delta; }
    }

    // sum of nums[0..i-1]
    int prefix(int i) {
        int sum = 0;
        for (; i > 0; i -= i & -i) { sum += tree[i]; }
        return sum;
    }

public:
    // adding values one by one. T: O(n log n)
    NumArray(const vector<int>& values) : n(values.size()), nums(n, 0), tree(n + 1, 0) {
        for (int i = 0; i < n; i++) { update(i, values[i]); }
    }

    // add the difference along the update path. T: O(log n)
    void update(int index, int val) {
        add(index, val - nums[index]);
        nums[index] = val;
    }

    // difference of two prefix sums. T: O(log n), S: O(n).
    int sumRange(int left, int right) {
        return prefix(right + 1) - prefix(left);
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    NumArray a({4, -1, 3, 2});
    check(a.sumRange(0, 3) == 8 && a.sumRange(1, 2) == 2, "initial");
    a.update(1, 5);
    check(a.sumRange(0, 1) == 9 && a.sumRange(0, 3) == 14, "update");
    a.update(1, 5);
    check(a.sumRange(2, 2) == 3, "single");
    a.update(3, -2);
    check(a.sumRange(1, 3) == 6, "repeat");
}
