// Cluster job scheduler (ByteDance compute-scheduling round; ASE compute platform).
// cap[i] = CPUs of node i. jobs[i] = {id, start, duration, cpu}, ids 0..J-1. Jobs are
// served FIFO by (start, id). The head job goes to the first node (lowest index) with
// enough free CPUs; if no node fits, it and everything behind it wait for the next job
// to finish. Return result[id] = {node, actual start}; {-1, -1} if cpu > every cap.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <deque>
#include <utility>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::deque;
using std::pair;
using std::sort;
using std::min;
using std::max_element;
using std::greater;

struct Job {
    int id;
    int start;
    int duration;
    int cpu;
};

bool byArrival(const Job& a, const Job& b) {
    if (a.start != b.start) { return a.start < b.start; }
    return a.id < b.id;
}

// Discrete-event simulation: time jumps to the earlier of the next arrival and the next
// completion; at each time, free finished jobs first, then place waiting jobs in order.
// T: O(J * (N + log J)), S: O(J + N).
vector<pair<int, long long>> schedule(const vector<int>& cap, vector<Job> jobs) {
    int numNodes = cap.size();
    int numJobs = jobs.size();
    sort(jobs.begin(), jobs.end(), byArrival);
    int maxCap = *max_element(cap.begin(), cap.end());

    // freeCpu[i] = idle CPUs on node i
    vector<int> freeCpu = cap;
    // result[id] = {node, start}
    vector<pair<int, long long>> result(numJobs, {-1, -1});
    // one global heap for the whole cluster: (end time, job id) of running jobs
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> running;
    // cpuOf[id] = CPUs that job holds while running
    vector<int> cpuOf(numJobs);
    // arrived but not placed yet, FIFO
    deque<Job> waiting;
    int nextArrival = 0;
    long long now = 0;

    while (nextArrival < numJobs || !waiting.empty()) {
        // 1. release every job finished by now
        while (!running.empty() && running.top().first <= now) {
            int id = running.top().second;
            running.pop();
            freeCpu[result[id].first] += cpuOf[id];
        }
        // 2. admit arrivals; a job bigger than every node can never run
        while (nextArrival < numJobs && jobs[nextArrival].start <= now) {
            if (jobs[nextArrival].cpu <= maxCap) {
                waiting.push_back(jobs[nextArrival]);
            }
            nextArrival++;
        }
        // 3. first fit for the head; stop at the first head that fits nowhere
        while (!waiting.empty()) {
            Job job = waiting.front();
            int node = -1;
            for (int i = 0; i < numNodes; i++) {
                if (freeCpu[i] >= job.cpu) {
                    node = i;
                    break;
                }
            }
            if (node == -1) { break; }
            freeCpu[node] -= job.cpu;
            cpuOf[job.id] = job.cpu;
            result[job.id] = {node, now};
            running.push({now + job.duration, job.id});
            waiting.pop_front();
        }
        // 4. jump to the next event
        long long nextTime = LLONG_MAX;
        if (!running.empty()) { nextTime = running.top().first; }
        if (nextArrival < numJobs) {
            nextTime = min(nextTime, (long long)jobs[nextArrival].start);
        }
        if (nextTime == LLONG_MAX) { break; }
        now = nextTime;
    }
    return result;
}

void test(const string& label, const vector<int>& cap, const vector<Job>& jobs,
          const vector<pair<int, long long>>& want) {
    cout << label << (schedule(cap, jobs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("fit-wait-never", {4, 2},
         {{0, 0, 5, 3}, {1, 0, 2, 2}, {2, 1, 1, 2}, {3, 1, 1, 8}},
         {{0, 0}, {1, 0}, {1, 2}, {-1, -1}});
    test("head-of-line", {4}, {{0, 0, 5, 3}, {1, 1, 1, 4}, {2, 2, 1, 1}},
         {{0, 0}, {0, 5}, {0, 6}});
    test("node-order", {2, 2}, {{0, 0, 3, 2}, {1, 0, 3, 2}, {2, 1, 1, 1}},
         {{0, 0}, {1, 0}, {0, 3}});
}

// follow-up: why one global heap, not a heap per node -> a blocked head waits for the
//   next completion anywhere in the cluster; a global (end, id) heap gives it in O(1)
// follow-up: head-of-line blocking -> backfill: a later job may start now if it fits and
//   ends before the head's earliest possible start (EASY backfilling)
// follow-up: best fit (less fragmentation) -> multiset<pair<free, node>>,
//   lower_bound({cpu, -1}) = tightest node, O(log N)
// follow-up: CPU + memory + GPU -> a node fits only if every resource fits
//   (all-or-nothing reserve); release returns all of them
