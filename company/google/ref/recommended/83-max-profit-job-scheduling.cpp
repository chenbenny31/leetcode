// LC 1235 Maximum Profit in Job Scheduling (Google, interview report). Jobs (start, end,
// profit) with start < end and profit >= 0, in any order. Choose jobs no two of which
// overlap; a job may start exactly when another ends. Return the maximum total profit
// (long long), 0 for no jobs.
//
// Clarify:
// - touching jobs (one ends when the next starts) -> compatible (stated)
// - profits up to ~1e9 over many jobs -> long long total
// - n up to ~5e4 -> O(n^2) is too slow, aim for O(n log n); times up to 1e9 -> no array
//   indexed by time
// - return only the max profit (the chosen jobs are a follow-up)
// Idea: sort by end; best[i] = max profit from the first i jobs. Job i is skipped
//   (best[i]) or taken on top of best[jobs ending <= its start]; those jobs form a
//   prefix, found with upper_bound on the sorted ends.
// Pitfalls:
// - upper_bound, not lower_bound: a job ending exactly at start is compatible
// - best has n + 1 entries; best[0] = 0 means "no job before"
// - sort by end, not start: the compatible jobs must form a prefix
// Tests to write: touching jobs; two short jobs beat one long; unsorted input; profits
//   past int; no jobs.

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

// straightforward: sort by end; job i is skipped (best[i]) or taken on top of the best
// prefix ending with a compatible job, found by scanning every earlier job.
// T: O(n^2), S: O(n).
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

// improved, "DP + binary search": best[] is non-decreasing, so the best compatible prefix
// is the longest one, i.e. all jobs with end <= start_i: upper_bound on the sorted ends.
// T: O(n log n), S: O(n).
long long maxProfit(vector<Job> jobs) {
    sortByEnd(jobs);
    int n = jobs.size();
    // ends[i] = end of job i in end order
    vector<int> ends;
    for (const Job& job : jobs) { ends.push_back(job.end); }
    // best[i] = max profit using only the first i jobs in end order
    vector<long long> best(n + 1, 0);
    for (int i = 0; i < n; i++) {
        // number of jobs ending at or before job i starts (all of them come before i)
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
    test("touching", {{1, 3, 50}, {3, 5, 40}}, 90);
    test("two-beat-one", {{1, 4, 50}, {1, 2, 30}, {2, 4, 30}}, 60);
    test("unsorted", {{4, 6, 70}, {2, 5, 60}, {1, 3, 20}}, 90);
    test("big-profit", {{1, 2, 2000000000}, {2, 3, 2000000000}}, 4000000000LL);
    test("no-jobs", {}, 0);
}

// follow-up: return the chosen jobs -> walk back from i = n: if best[i] == best[i - 1]
//   skip job i - 1, else take it and jump to i = its compatible count
// follow-up: all profits equal (max number of jobs) -> greedy by earliest end, O(n log n)
// follow-up: at most K jobs -> best[c][i] with c = jobs used, O(K n log n)
