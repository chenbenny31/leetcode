// LC 4068: meetings[i] = [start, end, revenue] on half-open [start, end). Pick a
// non-empty set of non-overlapping meetings. Earnings = their revenues + 1 per unit of
// idle time between consecutive picked meetings (nothing before the first or after the
// last). Return the max earnings.

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::max;
using std::upper_bound;

// The gaps add up to sum of (start of next - end of previous), so a meeting earns
// revenue, plus its start if it is not first, minus its end if it is not last.
// dp[i] = best total of a chain ending with meeting i, not yet charged its end:
// dp[i] = r_i + max(0, s_i + max over e_j <= s_i of (dp[j] - e_j)).

// meetings sorted by end
vector<vector<int>> byEnd(vector<vector<int>> meetings) {
    auto endsFirst = [](const vector<int>& a, const vector<int>& b) {
        return a[1] < b[1];
    };
    sort(meetings.begin(), meetings.end(), endsFirst);
    return meetings;
}

// straightforward: for each meeting, scan every earlier-ending one. T: O(n^2), S: O(n).
long long maxEarnings(const vector<vector<int>>& meetings) {
    vector<vector<int>> ms = byEnd(meetings);
    int n = ms.size();
    vector<long long> dp(n);
    long long best = LLONG_MIN;
    for (int i = 0; i < n; i++) {
        long long link = 0;
        for (int j = 0; j < i; j++) {
            if (ms[j][1] <= ms[i][0]) { link = max(link, ms[i][0] + dp[j] - ms[j][1]); }
        }
        dp[i] = ms[i][2] + link;
        best = max(best, dp[i]);
    }
    return best;
}

// improved: in end order, prefixBest[i] = max of dp[j] - e_j over the first i meetings;
// binary search finds how many end by s_i. T: O(n log n), S: O(n).
long long maxEarningsFast(const vector<vector<int>>& meetings) {
    vector<vector<int>> ms = byEnd(meetings);
    int n = ms.size();
    vector<int> ends(n);
    for (int i = 0; i < n; i++) { ends[i] = ms[i][1]; }
    vector<long long> prefixBest(n + 1, LLONG_MIN);
    long long best = LLONG_MIN;
    for (int i = 0; i < n; i++) {
        int cnt = upper_bound(ends.begin(), ends.end(), ms[i][0]) - ends.begin();
        long long link = 0;
        if (cnt > 0) { link = max(link, ms[i][0] + prefixBest[cnt]); }
        long long dp = ms[i][2] + link;
        best = max(best, dp);
        prefixBest[i + 1] = max(prefixBest[i], dp - ms[i][1]);
    }
    return best;
}

void test(const string& label, const vector<vector<int>>& meetings, long long want) {
    bool ok = maxEarnings(meetings) == want && maxEarningsFast(meetings) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {{0, 1, 5}}, 5);
    test("gap-bonus", {{0, 2, 1}, {5, 6, 1}}, 5);
    test("big-alone", {{0, 10, 10}, {1, 2, 1}, {3, 4, 1}}, 10);
    test("touching", {{0, 1, 1}, {1, 2, 1}}, 2);
    test("overlap", {{0, 3, 5}, {2, 4, 6}}, 6);
}
