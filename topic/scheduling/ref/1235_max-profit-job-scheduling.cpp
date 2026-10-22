// LC 1235 Maximum Profit in Job Scheduling: job i runs in [startTime[i], endTime[i])
// for profit[i]. Return the maximum profit from non-overlapping jobs (a job may start
// at the moment another ends).
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

// Weighted interval scheduling: sort by end, then best[i + 1] = max(skip job i, take it
// on top of the best over jobs finishing by its start, found by binary search).
// T: O(n log n), S: O(n).
int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
    int n = startTime.size();
    vector<vector<int>> jobs;   // {end, start, profit}, sorted by end
    for (int i = 0; i < n; i++) {
        jobs.push_back({endTime[i], startTime[i], profit[i]});
    }
    sort(jobs.begin(), jobs.end());
    vector<int> ends(n);        // ends[i] = end time of the i-th job in sorted order
    for (int i = 0; i < n; i++) { ends[i] = jobs[i][0]; }
    vector<int> best(n + 1, 0); // best[i] = max profit using only the first i jobs
    for (int i = 0; i < n; i++) {
        int k = upper_bound(ends.begin(), ends.begin() + i, jobs[i][1]) - ends.begin();
        best[i + 1] = max(best[i], best[k] + jobs[i][2]);
    }
    return best[n];
}

void test(const string& label, vector<int> startTime, vector<int> endTime,
          vector<int> profit, int want) {
    cout << label << (jobScheduling(startTime, endTime, profit) == want ? " PASS\n"
                                                                         : " FAIL\n");
}

int main() {
    test("touching", {1, 2, 3}, {3, 4, 5}, {5, 6, 4}, 9);
    test("big-overlap", {1, 3, 3}, {2, 4, 9}, {3, 4, 9}, 12);
    test("single", {1}, {2}, {7}, 7);
}
