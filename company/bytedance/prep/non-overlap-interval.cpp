// half-open of closed? e.g. [1,3] and [3,5] -> both stay
// return? min num removed
// sorted? no
// empty, neg vals? allowed
// start < end guaranteed? yes

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;


int minimumIntervalRemovals(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; }); // sort by incre ends
    int n = intervals.size();
    if (n == 0) { return 0; }

    int keep = 1; // min-end interval always keep
    int lastEnd = intervals[0][1];
    for (int i = 1; i < n; i++) {
        if (intervals[i][0] >= lastEnd) {
            keep++;
            lastEnd = intervals[i][1];
        }
    }
    return n - keep;
}

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    cout << label << (minimumIntervalRemovals(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("touching", {{1, 3}, {3, 5}, {5, 7}}, 0);
    test("nested", {{1, 10}, {2, 3}, {4, 5}}, 1);
    test("dups", {{0, 2}, {0, 2}}, 1);
    test("negative", {{-3, -1}, {-2, 0}, {-1, 1}}, 1);
    test("empty", {}, 0);
}
