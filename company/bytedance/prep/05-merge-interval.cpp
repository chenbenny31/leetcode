// closed or half-open? e.g. [1,4], [4,5] merge -> [1,5]
// sorted? No
// any start > end? No
// input be empty? Yes
// output be sorted? yes
// allow modify in-place?

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::max;
using std::min;

// sort by start, then extend the last merged interval, T: O(nlogn), S: O(n)
vector<vector<int>> merge(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    vector<vector<int>> merged;
    for (const vector<int>& cur : intervals) {
        if (!merged.empty() && cur[0] <= merged.back()[1]) { // overlap -> merge
            merged.back()[1] = max(merged.back()[1], cur[1]); 
        } else {
            merged.push_back(cur);
        }
    }
    return merged;
}

// follow-up: insert, copy ending before, abosert overlaps, copy the rest
// T: O(n), S: O(n)
vector<vector<int>> insert(const vector<vector<int>>& intervals,
                           vector<int> newInterval) {
    vector<vector<int>> result;
    int n = intervals.size();
    int i = 0;

    while (i < n && intervals[i][1] < newInterval[0]) {
        result.push_back(intervals[i]);
        i++;
    }

    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    result.push_back(newInterval);

    while (i < n) {
        result.push_back(intervals[i]);
        i++;
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& intervals,
          const vector<vector<int>>& want) {
    cout << label << (merge(intervals) == want ? " PASS\n" : " FAIL\n");
}

void testInsert(const string& label, const vector<vector<int>>& intervals,
                const vector<int>& newInterval, const vector<vector<int>>& want) {
    cout << label << (insert(intervals, newInterval) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {{1, 3}, {2, 6}, {8, 10}, {15, 18}}, {{1, 6}, {8, 10}, {15, 18}});
    test("touching", {{1, 4}, {4, 5}}, {{1, 5}});
    test("unsorted nested", {{6, 8}, {1, 9}, {2, 4}, {4, 7}}, {{1, 9}});
    test("empty", {}, {});
    testInsert("insert spans middle", {{1, 2}, {3, 5}, {6, 7}, {8, 10}, {12, 16}},
               {4, 8}, {{1, 2}, {3, 10}, {12, 16}});
    testInsert("insert at end", {{1, 5}}, {6, 8}, {{1, 5}, {6, 8}});
}
