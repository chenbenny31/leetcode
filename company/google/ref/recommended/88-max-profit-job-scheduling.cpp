// LC 1235 Maximum Profit in Job Scheduling (Google, interview report)
// jobs (start, end, profit), start < end, profit >= 0, any order
// pick non-overlapping jobs; return max total profit (long long), 0 for no jobs
//
// clarify:
// touching jobs (end == start) -> compatible
// profit range -> total in long long
// n -> ~5e4, need O(n log n)
// time range -> large, no array indexed by time
// profit only or the chosen jobs -> profit; jobs = follow-up

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::max;
using std::upper_bound;

struct Job {
    int start;
    int end;
    long long profit;
};

void sortByEnd(vector<Job>& jobs) {
    sort(jobs.begin(), jobs.end(),
         [](const Job& a, const Job& b) { return a.end < b.end; });
}

// solution 1: DP w/ linear scan; state it first
// core idea: sort by end; job i skipped (best[i]) or taken on top of a compatible prefix
// compatible prefix found by scanning every earlier job
// T: O(n^2), S: O(n)
long long maxProfitScan(vector<Job> jobs) {
    sortByEnd(jobs);
    int n = jobs.size();
    // best[i] = max profit using only the first i jobs in end order
    vector<long long> best(n + 1, 0);
    for (int i = 0; i < n; i++) {
        long long take = jobs[i].profit;
        for (int before = 0; before < i; before++) {
            if (jobs[before].end <= jobs[i].start) {
                take = max(take, jobs[i].profit + best[before + 1]);
            }
        }
        best[i + 1] = max(best[i], take);
    }
    return best[n];
}

// solution 2: DP + binary search; the expected answer
// core idea: best[] is non-decreasing -> best compatible prefix = the longest one
// longest = all jobs w/ end <= start_i -> upper_bound on the sorted ends
// T: O(n log n), S: O(n)
long long maxProfit(vector<Job> jobs) {
    sortByEnd(jobs);
    int n = jobs.size();
    // ends[i] = end of job i in end order
    vector<int> ends;
    for (const Job& job : jobs) { ends.push_back(job.end); }
    // best[i] = max profit using only the first i jobs in end order
    vector<long long> best(n + 1, 0);
    for (int i = 0; i < n; i++) {
        // jobs ending at or before job i starts; upper_bound: a touching job counts
        int compatible =
            upper_bound(ends.begin(), ends.end(), jobs[i].start) - ends.begin();
        best[i + 1] = max(best[i], jobs[i].profit + best[compatible]);
    }
    return best[n];
}

void test(const string& label, const vector<Job>& jobs, long long want) {
    bool ok = maxProfitScan(jobs) == want && maxProfit(jobs) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-beat-one", {{1, 4, 50}, {1, 2, 30}, {2, 4, 30}}, 60);
    test("unsorted", {{4, 6, 70}, {2, 5, 60}, {1, 3, 20}}, 90);
    test("big-profit", {{1, 2, 2000000000}, {2, 3, 2000000000}}, 4000000000LL);
    test("no-jobs", {}, 0);
}

// follow-up:
// return the chosen jobs -> walk back from i = n: best[i] == best[i-1] -> skip job i-1
// walk back, job taken -> jump to i = its compatible cnt
// all profits equal (max num of jobs) -> greedy by earliest end, O(n log n)
// at most K jobs -> best[c][i] w/ c = jobs used, O(K * n log n)
