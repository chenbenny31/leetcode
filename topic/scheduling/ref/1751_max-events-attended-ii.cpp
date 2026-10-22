// LC 1751 Maximum Number of Events That Can Be Attended II: events[i] = {start, end,
// value}, inclusive days. You attend whole events, one at a time, at most k of them.
// Return the maximum total value.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::upper_bound;
using std::max;

// Sort by start; for event i either skip it or attend it and jump to the first event
// starting after its end (binary search). T: O(n log n + n*k), S: O(n*k).
int maxValue(vector<vector<int>>& events, int k) {
    int n = events.size();
    sort(events.begin(), events.end());
    vector<int> starts(n);   // starts[i] = start day of the i-th event in sorted order
    for (int i = 0; i < n; i++) { starts[i] = events[i][0]; }
    // dp[i][j] = max value from events i.. when at most j more may be attended
    vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
    for (int i = n - 1; i >= 0; i--) {
        int next = upper_bound(starts.begin(), starts.end(), events[i][1])
                   - starts.begin();
        for (int j = 1; j <= k; j++) {
            dp[i][j] = max(dp[i + 1][j], events[i][2] + dp[next][j - 1]);
        }
    }
    return dp[0][k];
}

void test(const string& label, vector<vector<int>> events, int k, int want) {
    cout << label << (maxValue(events, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("k-limits", {{1, 3, 5}, {4, 5, 6}, {2, 6, 9}}, 1, 9);
    test("two-best", {{1, 3, 5}, {4, 5, 6}, {2, 6, 9}}, 2, 11);
    test("touching-conflict", {{1, 2, 3}, {2, 3, 4}}, 2, 4);
}
