// LC 252 Meeting Rooms (premium): given meeting intervals [start, end), can one person
// attend all of them? A meeting may start exactly when another ends.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

// Sort by start; any meeting starting before the previous one ends is a clash.
// T: O(n log n), S: O(1).
bool canAttendMeetings(vector<vector<int>> intervals) {
    int n = intervals.size();
    sort(intervals.begin(), intervals.end());
    for (int i = 1; i < n; i++) {
        if (intervals[i][0] < intervals[i - 1][1]) { return false; }
    }
    return true;
}

void test(const string& label, const vector<vector<int>>& intervals, bool want) {
    cout << label << (canAttendMeetings(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("disjoint", {{5, 8}, {1, 3}}, true);
    test("overlap", {{1, 4}, {3, 5}}, false);
    test("touching", {{1, 2}, {2, 3}}, true);
    test("empty", {}, true);
}
