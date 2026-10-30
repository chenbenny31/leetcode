// n stations 0..n-1 sit on a ring; moving to an adjacent station costs 1. A drone starts
// at start and visits targets in the given order, going either way around each time.
// Return the minimum total time.

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::abs;
using std::min;

// each leg independently takes the shorter direction. T: O(t) for t targets, S: O(1).
long long minTravel(int n, int start, const vector<int>& targets) {
    long long total = 0;
    int cur = start;
    for (int nxt : targets) {
        int gap = abs(nxt - cur);
        total += min(gap, n - gap);
        cur = nxt;
    }
    return total;
}

void test(const string& label, int n, int start, const vector<int>& targets,
          long long want) {
    cout << label << (minTravel(n, start, targets) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("wrap", 6, 0, {5}, 1);
    test("half", 4, 0, {2}, 2);
    test("chain", 5, 0, {2, 4, 0}, 5);
    test("stay", 3, 1, {1}, 0);
    test("one-station", 1, 0, {0, 0}, 0);
}
