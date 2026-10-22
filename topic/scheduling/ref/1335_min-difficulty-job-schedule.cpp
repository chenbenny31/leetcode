// LC 1335 Minimum Difficulty of a Job Schedule: split jobs, which must be done in order,
// into d non-empty consecutive days; a day costs its hardest job. Return the minimum
// total cost, or -1 if there are fewer jobs than days.
#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// DP over (days used, jobs done); the last day takes jobs j..i-1, and the hardest job
// is tracked while j walks left. T: O(d*n^2), S: O(d*n).
int minDifficulty(vector<int>& jobDifficulty, int d) {
    int n = jobDifficulty.size();
    if (n < d) { return -1; }
    int inf = INT_MAX / 2;
    // dp[k][i] = min total cost of the first i jobs spread over exactly k days
    vector<vector<int>> dp(d + 1, vector<int>(n + 1, inf));
    dp[0][0] = 0;
    for (int k = 1; k <= d; k++) {
        for (int i = k; i <= n; i++) {
            int hardest = 0;
            for (int j = i - 1; j >= k - 1; j--) {
                hardest = max(hardest, jobDifficulty[j]);
                dp[k][i] = min(dp[k][i], dp[k - 1][j] + hardest);
            }
        }
    }
    return dp[d][n];
}

void test(const string& label, vector<int> jobDifficulty, int d, int want) {
    cout << label << (minDifficulty(jobDifficulty, d) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("split", {1, 9, 2}, 2, 10);
    test("one-per-day", {3, 1, 2}, 3, 6);
    test("one-day", {2, 7, 4}, 1, 7);
    test("too-few-jobs", {5}, 2, -1);
}
