// Cluster first-fit scheduler (Medium).
// Pattern: event simulation + global min-heap of end times.
// cap[i] = CPUs of node i; jobs[id] = {submit, duration, cpu}, FIFO by (submit, id).
// The head job takes the lowest-index node with enough free CPUs, else it and all behind
// it wait until running jobs finish. Every job fits on some node. Return start times.

#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using std::cout;
using std::greater;
using std::min;
using std::pair;
using std::priority_queue;
using std::queue;
using std::sort;
using std::string;
using std::vector;

// lowest-index node with at least `need` idle CPUs, or -1
int firstFit(const vector<int>& freeCpu, int need) {
    int numNodes = freeCpu.size();
    for (int i = 0; i < numNodes; i++) {
        if (freeCpu[i] >= need) { return i; }
    }
    return -1;
}

// Time jumps to the next submit or completion; at each time release finished jobs, admit
// submits, then place waiting jobs in FIFO order. T: O(J * (N + log J)), S: O(J + N).
vector<long long> schedule(const vector<int>& cap, const vector<vector<int>>& jobs) {
    int numJobs = jobs.size();
    // submit order: (submit, id)
    vector<pair<int, int>> order;
    for (int id = 0; id < numJobs; id++) { order.push_back({jobs[id][0], id}); }
    sort(order.begin(), order.end());
    // freeCpu[i] = idle CPUs on node i
    vector<int> freeCpu = cap;
    // start[id] = start time of job id
    vector<long long> start(numJobs);
    // nodeOf[id] = node that runs job id
    vector<int> nodeOf(numJobs);
    // running jobs across the whole cluster, earliest end first: (end time, id)
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> running;
    // ids submitted but not placed yet, FIFO
    queue<int> waiting;
    // order[next] = next job to submit
    int next = 0;
    long long now = 0;
    while (next < numJobs || !waiting.empty()) {
        while (!running.empty() && running.top().first <= now) {
            int id = running.top().second;
            running.pop();
            freeCpu[nodeOf[id]] += jobs[id][2];
        }
        while (next < numJobs && order[next].first <= now) {
            waiting.push(order[next].second);
            next++;
        }
        while (!waiting.empty()) {
            int id = waiting.front();
            int cpu = jobs[id][2];
            int node = firstFit(freeCpu, cpu);
            // head-of-line blocking: nobody behind the head may start
            if (node == -1) { break; }
            waiting.pop();
            freeCpu[node] -= cpu;
            nodeOf[id] = node;
            start[id] = now;
            running.push({now + jobs[id][1], id});
        }
        // a blocked head always has a running job to wait for, so nextTime stays finite
        long long nextTime = LLONG_MAX;
        if (!running.empty()) { nextTime = running.top().first; }
        if (next < numJobs) { nextTime = min(nextTime, (long long)order[next].first); }
        now = nextTime;
    }
    return start;
}

void test(const string& label, const vector<int>& cap, const vector<vector<int>>& jobs,
          const vector<long long>& want) {
    cout << label << (schedule(cap, jobs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("wait-for-release", {4, 4}, {{0, 13, 3}, {0, 5, 3}, {1, 1, 2}, {1, 2, 2}},
         {0, 0, 5, 5});
    test("fits-on-arrival", {4, 4}, {{0, 3, 4}, {2, 1, 4}}, {0, 2});
    test("head-of-line", {4}, {{0, 5, 3}, {1, 1, 4}, {2, 1, 1}}, {0, 5, 6});
}

// follow-up: head-of-line blocking -> backfill: a later job may start now if it fits and
//   ends before the head's earliest possible start
// follow-up: best fit -> set<pair<free, node>>, lower_bound({cpu, -1}) = tightest node
