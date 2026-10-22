// LC 1353 Maximum Number of Events That Can Be Attended: events[i] = {start, end} days
// inclusive. You attend at most one event per day, on any day within its range.
// Return the maximum number of events you can attend.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::sort;

// Sweep days; each day attend the open event with the earliest end (heap of ends).
// Jump over empty days. T: O(n log n), S: O(n).
int maxEvents(vector<vector<int>> events) {
    int n = events.size();
    sort(events.begin(), events.end());
    // end days of events that have started and not yet been attended, earliest first
    priority_queue<int, vector<int>, greater<int>> open;
    int i = 0;
    int day = 0;
    int attended = 0;
    while (i < n || !open.empty()) {
        if (open.empty()) { day = events[i][0]; }
        while (i < n && events[i][0] <= day) {
            open.push(events[i][1]);
            i++;
        }
        while (!open.empty() && open.top() < day) { open.pop(); }
        if (!open.empty()) {
            open.pop();
            attended++;
            day++;
        }
    }
    return attended;
}

void test(const string& label, const vector<vector<int>>& events, int want) {
    cout << label << (maxEvents(events) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {{1, 2}, {2, 3}, {3, 4}}, 3);
    test("same-day", {{1, 1}, {1, 1}}, 1);
    test("squeeze", {{1, 3}, {1, 3}, {1, 3}, {3, 3}}, 3);
    test("gap", {{1, 2}, {5, 6}}, 2);
}
