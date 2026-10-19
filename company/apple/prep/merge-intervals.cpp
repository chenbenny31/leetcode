// merge all overlapping closed intervals [start, end]
// border inclusive: merge [1,2] and [2,3]

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::max;

// sort by start, extend the last merged interval, T: O(nlogn), S: O(n)
vector<vector<int>> merge(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    vector<vector<int>> merged;
    for (const auto& cur : intervals) {
        if (!merged.empty() && cur[0] <= merged.back()[1]) {
            merged.back()[1] = max(merged.back()[1], cur[1]);
        } else {
            merged.push_back(cur);
        }
    }
    return merged;
}

void test(const string& label, const vector<vector<int>>& intervals,
          const vector<vector<int>>& want) {
    cout << label << (merge(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("unsorted", {{8, 10}, {1, 3}, {2, 6}}, {{1, 6}, {8, 10}});
    test("touching", {{1, 2}, {2, 3}}, {{1, 3}});
    test("contained", {{1, 10}, {2, 3}}, {{1, 10}});
    test("empty", {}, {});
}
