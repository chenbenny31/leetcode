// One operation adds a positive x to every element of a contiguous subarray. Return the
// minimum total of all x used so that the array becomes non-decreasing.
// (Same answer as "sum of all drops a[i-1] - a[i] > 0".)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Adding x to [l, r] raises the step a[l] - a[l-1] by x and lowers a[r+1] - a[r] by x,
// so it can fix at most x of one drop; adding to a suffix fixes exactly that much and
// breaks nothing. So the answer is the sum of all drops. T: O(n), S: O(1).
long long minIncrements(const vector<int>& a) {
    long long total = 0;
    int n = a.size();
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) { total += a[i - 1] - a[i]; }
    }
    return total;
}

void test(const string& label, const vector<int>& a, long long want) {
    cout << label << (minIncrements(a) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("sorted", {1, 2, 3}, 0);
    test("one-drop", {3, 1, 2}, 2);
    test("two-drops", {4, 1, 5, 2}, 6);
    test("falling", {3, 2, 1}, 2);
    test("single", {7}, 0);
}
