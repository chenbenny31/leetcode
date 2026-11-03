// ByteDance (FastPrep, phone / onsite, 2 sightings) / LC 435: remove the fewest
// intervals so the rest are pairwise non-overlapping; intervals that only touch at an
// endpoint may stay together. Return the minimum number removed. (The HM asked why
// sorting by end time gives the optimal greedy — proof below.)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Greedy: sort by end and keep every interval that starts at or after the last kept
// end; removed = n - kept.
// Why it is optimal (exchange argument): let g be the interval with the smallest end.
// Take any optimal set and its interval o with the smallest end. Since end(g) <=
// end(o), swapping o for g overlaps nothing new, so some optimal set contains g. Remove
// g and everything overlapping it, and repeat the argument on what is left.
// T: O(n log n), S: O(1) besides the sort.
int minimumIntervalRemovals(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    int kept = 0;
    long long lastEnd = -(1LL << 40);
    for (const auto& iv : intervals) {
        if (iv[0] >= lastEnd) {
            kept++;
            lastEnd = iv[1];
        }
    }
    return intervals.size() - kept;
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
