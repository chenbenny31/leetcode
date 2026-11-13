// LC 2747 Count Zero Request Servers
// (1) logs[i] = {serverId, time}; per query q: num of servers w/ no request in [q - x, q]
// (2) online queries in any order
// (3) stream: requests and queries interleaved in time order
//
// clarify:
// window -> inclusive at both ends
// logs -> unsorted, many per server
// queries may repeat -> answers follow input order
// all queries known up front -> offline sort; else follow-up (2) / (3)

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::unordered_set;
using std::unordered_map;
using std::sort;
using std::iota;
using std::lower_bound;
using std::upper_bound;

// (1) solution 1: brute force; baseline
// core idea: per query, collect the servers w/ a log inside the window
// T: O(Q * L), S: O(n)
vector<int> countServers(int n, const vector<vector<int>>& logs, int x,
                         const vector<int>& queries) {
    vector<int> answer;
    for (int q : queries) {
        // servers with at least one request in [q - x, q]
        unordered_set<int> active;
        for (const auto& log : logs) {
            if (q - x <= log[1] && log[1] <= q) { active.insert(log[0]); }
        }
        answer.push_back(n - (int)active.size());
    }
    return answer;
}

// (1) solution 2: offline sliding window; use when all queries are known
// core idea: sort logs by time, visit queries by increasing q -> window only slides right
// keep a per-server cnt and num of active servers; each log enters and leaves once
// T: O(L log L + Q log Q), S: O(n + L + Q)
vector<int> countServersFast(int n, vector<vector<int>> logs, int x,
                             const vector<int>& queries) {
    sort(logs.begin(), logs.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    int numQueries = queries.size();
    // query indices sorted by query value
    vector<int> order(numQueries);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(),
         [&](int a, int b) { return queries[a] < queries[b]; });
    // count[serverId] = requests of serverId inside the window
    unordered_map<int, int> count;
    // servers with count > 0
    int active = 0;
    // window = logs[left..right-1]
    int left = 0;
    int right = 0;
    int numLogs = logs.size();
    vector<int> answer(numQueries);
    for (int index : order) {
        int q = queries[index];
        while (right < numLogs && logs[right][1] <= q) {
            int id = logs[right][0];
            count[id]++;
            if (count[id] == 1) { active++; }
            right++;
        }
        while (left < right && logs[left][1] < q - x) {
            int id = logs[left][0];
            count[id]--;
            if (count[id] == 0) { active--; }
            left++;
        }
        answer[index] = n - active;
    }
    return answer;
}

// (2) online queries
// core idea: sort logs by time once; per query binary search the slice, cnt distinct ids
// T: O(L log L) once + O(log L + W) per query, S: O(n + L) — W = logs in the window
vector<int> countServersOnline(int n, vector<vector<int>> logs, int x,
                               const vector<int>& queries) {
    sort(logs.begin(), logs.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    // times[i] = time of logs[i], ascending
    vector<int> times;
    for (const auto& log : logs) { times.push_back(log[1]); }
    vector<int> answer;
    for (int q : queries) {
        int first = lower_bound(times.begin(), times.end(), q - x) - times.begin();
        int last = upper_bound(times.begin(), times.end(), q) - times.begin();
        // servers with a log in [q - x, q]
        unordered_set<int> active;
        for (int i = first; i < last; i++) { active.insert(logs[i][0]); }
        answer.push_back(n - (int)active.size());
    }
    return answer;
}

// (3) stream
// core idea: queue of logs that can still be in a window + same cnt / active bookkeeping
// T: O(1) amortized per call, S: O(n + W) — each log is pushed and popped once
class ZeroRequestMonitor {
    // (time, serverId) of logs that may still be inside a window, oldest first
    queue<pair<int, int>> window;
    // count[serverId] = logs of serverId in window
    unordered_map<int, int> count;
    // servers with count > 0
    int active = 0;
    int n;
    int x;

public:
    ZeroRequestMonitor(int n, int x) : n(n), x(x) {}

    void record(int serverId, int time) {
        window.push({time, serverId});
        count[serverId]++;
        if (count[serverId] == 1) { active++; }
    }

    int query(int q) {
        while (!window.empty() && window.front().first < q - x) {
            int id = window.front().second;
            window.pop();
            count[id]--;
            if (count[id] == 0) { active--; }
        }
        return n - active;
    }
};

void test(const string& label, int n, const vector<vector<int>>& logs, int x,
          const vector<int>& queries, const vector<int>& want) {
    bool ok = countServers(n, logs, x, queries) == want;
    ok = ok && countServersFast(n, logs, x, queries) == want;
    ok = ok && countServersOnline(n, logs, x, queries) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("no-logs", 2, {}, 1, {5}, {2});
    test("window-edges", 3, {{1, 2}, {2, 5}, {3, 9}}, 3, {5, 8, 9}, {1, 2, 2});
    test("repeat-server", 2, {{1, 1}, {1, 2}, {1, 3}}, 1, {3, 5, 2}, {1, 2, 1});
    test("unsorted-logs", 3, {{3, 7}, {1, 1}, {2, 4}}, 3, {7, 1}, {1, 2});

    ZeroRequestMonitor monitor(3, 2);
    monitor.record(1, 1);
    monitor.record(2, 2);
    check(monitor.query(2) == 1, "stream-two-busy");
    monitor.record(1, 4);
    check(monitor.query(4) == 1, "stream-edge-stays");
    check(monitor.query(5) == 2, "stream-evict");
    check(monitor.query(9) == 3, "stream-all-idle");
}

// follow-up:
// logs known, queries arrive in increasing time -> countServersFast w/o sorting queries
// guaranteed O(log L) per online query -> persistent segment tree (say it, don't code it)
// return the idle server ids -> cnt as vector<int>(n + 1); idle = ids w/ cnt 0, O(n) each
