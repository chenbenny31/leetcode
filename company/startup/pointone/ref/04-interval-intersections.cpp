// LC 986: two lists of closed intervals, each sorted and pairwise disjoint. Return their
// intersection (also sorted and disjoint). [1,2] and [2,3] intersect at [2,2].
// PointOne angle: activity log intersected with billable windows.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// straightforward: test every pair. Output stays sorted because both lists are sorted
// and disjoint. T: O(m*n), S: O(1) + output.
vector<vector<int>> intervalIntersection(const vector<vector<int>>& a,
                                         const vector<vector<int>>& b) {
    vector<vector<int>> result;
    for (const auto& x : a) {
        for (const auto& y : b) {
            int lo = max(x[0], y[0]), hi = min(x[1], y[1]);
            if (lo <= hi) { result.push_back({lo, hi}); }
        }
    }
    return result;
}

// improved: two pointers; the interval that ends first cannot meet anything later in the
// other list, so advance it. T: O(m+n), S: O(1) + output.
vector<vector<int>> intervalIntersectionTwoPtr(const vector<vector<int>>& a,
                                               const vector<vector<int>>& b) {
    vector<vector<int>> result;
    int m = a.size(), n = b.size();
    int i = 0, j = 0;
    while (i < m && j < n) {
        int lo = max(a[i][0], b[j][0]), hi = min(a[i][1], b[j][1]);
        if (lo <= hi) { result.push_back({lo, hi}); }
        if (a[i][1] < b[j][1]) {
            i++;
        } else {
            j++;
        }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& a, const vector<vector<int>>& b,
          const vector<vector<int>>& want) {
    bool ok = intervalIntersection(a, b) == want &&
              intervalIntersectionTwoPtr(a, b) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {{1, 3}, {5, 7}}, {{2, 6}}, {{2, 3}, {5, 6}});
    test("point", {{1, 2}}, {{2, 3}}, {{2, 2}});
    test("none", {{1, 2}}, {{3, 4}}, {});
    test("empty", {}, {{1, 2}}, {});
}
