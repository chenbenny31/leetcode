// LC 2747 Count Zero Request Servers. Servers are 1..n; logs[i] = {serverId, time} means
// that server got a request at that time; logs are in any order. For each query q, return
// how many servers got no request in the inclusive window [q - x, q]. Answers follow the
// input order of queries.
//
// Clarify:
// - window inclusive at both ends: a log at q - x or at q counts
// - logs unsorted, a server may have many logs, a server with no log at all is idle
// - n, L (logs), Q (queries) up to ~1e5 -> O(Q * L) is 1e10: too slow; all queries are
//   known up front, so answer them offline in sorted order
// - answers in the input order of queries (queries may repeat)
// - times up to ~1e6 and x < q -> no overflow or negative window start
// Idea: sort logs by time and visit queries by increasing q. Both window ends then only
//   move right, so each log enters and leaves the window once. A per-server count in the
//   window plus the number of servers with count > 0 gives n - active per query.
// Pitfalls:
// - add logs with time <= q, drop logs with time < q - x (both ends inclusive)
// - active changes only when a count goes 0 -> 1 or 1 -> 0, not per log
// - write answer[index] for the original index, not push_back in sorted order
// - sort the logs by time, not by server id (the default vector order)
// Tests to write: no logs -> n; a log exactly at q - x and at q; one server with many
//   logs (counted once, leaves only when its last log does); unsorted logs and queries.

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

// straightforward: for each query, collect the servers with a log inside the window.
// T: O(Q * L), S: O(n).
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

// improved, offline: all queries are known up front, so answer them in increasing q,
// not input order; the window [q - x, q] then only slides right over the time-sorted
// logs, and each log enters and leaves it once. T: O(L log L + Q log Q), S: O(n + L + Q).
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

// follow-up: online queries in any order (each answered on arrival) -> sort the logs by
// time once; per query binary search the slice of logs inside [q - x, q] and count the
// distinct servers in it. T: O(L log L) once + O(log L + W) per query, W = logs in the
// window; S: O(n + L).
// Pitfall: lower_bound for q - x and upper_bound for q, so both ends stay inclusive.
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

// follow-up (stream): requests and queries arrive interleaved in time order; query(q)
// asks about [q - x, q] at the current time q (every recorded time is <= q). Keep the
// logs that can still be in a window in a queue, with countServersFast's bookkeeping.
// T: O(1) amortized per call, S: O(n + logs in the window).
// Pitfalls: evict in query, with < q - x so a log at q - x stays; a server stays active
//   until its last log leaves.
// Tests to write: two busy servers; a log at q - x still counts; eviction with a server
//   that has a newer log; everything evicted -> n.
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

// follow-up: logs known, queries arrive in increasing time -> countServersFast without
//   sorting the queries (loop over queries directly); logs streaming in too ->
//   ZeroRequestMonitor above
// follow-up: online queries in any order -> countServersOnline above; a guaranteed
//   O(log L) per query needs a persistent segment tree over "previous log of the same
//   server" (rarely expected; say it, don't code it)
// follow-up: return the idle server ids -> count can live in a vector<int>(n + 1); the
//   idle ones are the ids with count[id] == 0 (O(n) per query to list them)
