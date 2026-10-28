// One operation picks values x and y and changes every x into y. Return the min number of
// operations so that every value that remains occupies one contiguous block.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::max;

// Overlapping [first, last] spans form independent segments; a segment with d distinct
// values needs d - 1 merges. T: O(n), S: O(n).
int minOps(const vector<int>& a) {
    int n = a.size();
    // last[x] = last index of x
    unordered_map<int, int> last;
    for (int i = 0; i < n; i++) { last[a[i]] = i; }
    int ops = 0, end = 0;
    // distinct values in the current segment
    unordered_set<int> seen;
    for (int i = 0; i < n; i++) {
        end = max(end, last[a[i]]);
        seen.insert(a[i]);
        if (i == end) {
            ops += seen.size() - 1;
            seen.clear();
        }
    }
    return ops;
}

void test(const string& label, const vector<int>& a, int want) {
    cout << label << (minOps(a) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("already", {1, 1, 2}, 0);
    test("interleave", {1, 2, 1, 2}, 1);
    test("chain", {1, 2, 1, 3, 2}, 2);
    test("split", {1, 2, 1, 3, 3}, 1);
    test("single", {5}, 0);
}
