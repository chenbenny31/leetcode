// Logger Dedup, two parts (Google, interview report). Window rule for both parts: two
// copies of a message are duplicates iff |t1 - t2| < 10 (exactly 10 apart is fine).
// (1) Out-of-order rate limiter: (timestamp, message) arrive in any time order. Print
//     a message iff no PRINTED copy is within the window; only printed copies are
//     recorded.
// (2) Delayed duplicate suppression: logs arrive sorted by time. Output a log iff no
//     other log with the same message is within the window, before or after it. Output
//     order = time order.

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <unordered_map>
#include <utility>
#include <iterator>

using std::cout;
using std::string;
using std::vector;
using std::set;
using std::queue;
using std::unordered_map;
using std::pair;
using std::prev;

const int WINDOW = 10;

// (1) per message, an ordered set of printed times; only the nearest printed time on
// each side can be within the window. T: O(log k) per call, S: O(printed logs).
class Logger {
    // message -> timestamps at which it was printed
    unordered_map<string, set<int>> printed;

public:
    bool shouldPrint(int timestamp, const string& message) {
        set<int>& times = printed[message];
        auto it = times.lower_bound(timestamp);
        if (it != times.end() && *it - timestamp < WINDOW) { return false; }
        if (it != times.begin() && timestamp - *prev(it) < WINDOW) { return false; }
        times.insert(timestamp);
        return true;
    }
};

// (2) offline, two passes: compare each log with the previous and the next occurrence of
// its message. T: O(n), S: O(n).
vector<string> uniqueLogs(const vector<pair<int, string>>& logs) {
    int n = logs.size();
    // keep[i] = log i has no same-message neighbor within the window
    vector<bool> keep(n, true);
    // message -> time of its latest occurrence seen so far in the current pass
    unordered_map<string, int> last;
    for (int i = 0; i < n; i++) {
        const auto& [time, message] = logs[i];
        auto it = last.find(message);
        if (it != last.end() && time - it->second < WINDOW) { keep[i] = false; }
        last[message] = time;
    }
    last.clear();
    for (int i = n - 1; i >= 0; i--) {
        const auto& [time, message] = logs[i];
        auto it = last.find(message);
        if (it != last.end() && it->second - time < WINDOW) { keep[i] = false; }
        last[message] = time;
    }
    vector<string> out;
    for (int i = 0; i < n; i++) {
        if (keep[i]) { out.push_back(logs[i].second); }
    }
    return out;
}

struct Pending {
    int time;
    string message;
    // arrival number of this log
    int seq;
    // an earlier copy is within the window
    bool dup;
};

// (2) streaming: hold each log until time + WINDOW has arrived; by then every later copy
// within the window has been seen. T: O(1) per log, S: O(logs within one window).
class Dedup {
    // logs not yet released, in time order
    queue<Pending> pending;
    // message -> (time, seq) of its latest pending occurrence
    unordered_map<string, pair<int, int>> latest;
    int nextSeq = 0;

    // release logs with time + WINDOW <= now
    vector<string> release(int now) {
        vector<string> out;
        while (!pending.empty() && pending.front().time + WINDOW <= now) {
            Pending log = pending.front();
            pending.pop();
            // a later copy exists iff this log is no longer the latest one
            if (latest[log.message].second != log.seq) { continue; }
            latest.erase(log.message);
            if (!log.dup) { out.push_back(log.message); }
        }
        return out;
    }

public:
    // returns the logs released by this arrival
    vector<string> push(int timestamp, const string& message) {
        vector<string> out = release(timestamp);
        auto it = latest.find(message);
        bool dup = it != latest.end() && timestamp - it->second.first < WINDOW;
        latest[message] = {timestamp, nextSeq};
        pending.push({timestamp, message, nextSeq, dup});
        nextSeq++;
        return out;
    }

    // end of stream: release everything left
    vector<string> finish() {
        if (pending.empty()) { return {}; }
        return release(pending.back().time + WINDOW);
    }
};

void testLogger(const string& label, const vector<pair<int, string>>& calls,
                const vector<bool>& want) {
    Logger logger;
    vector<bool> got;
    for (const auto& [timestamp, message] : calls) {
        got.push_back(logger.shouldPrint(timestamp, message));
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

void testDedup(const string& label, const vector<pair<int, string>>& logs,
               const vector<string>& want) {
    Dedup dedup;
    vector<string> streamed;
    for (const auto& [timestamp, message] : logs) {
        for (const string& out : dedup.push(timestamp, message)) {
            streamed.push_back(out);
        }
    }
    for (const string& out : dedup.finish()) { streamed.push_back(out); }
    bool ok = uniqueLogs(logs) == want && streamed == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    testLogger("boundary", {{1, "foo"}, {5, "foo"}, {11, "foo"}, {11, "bar"}},
               {true, false, true, true});
    testLogger("not-recorded", {{0, "foo"}, {9, "foo"}, {15, "foo"}},
               {true, false, true});
    testLogger("out-of-order",
               {{10, "foo"}, {1, "foo"}, {20, "foo"}, {0, "foo"}, {15, "foo"}},
               {true, false, true, true, false});

    testDedup("both-dropped", {{0, "foo"}, {5, "foo"}, {5, "bar"}}, {"bar"});
    testDedup("keep-boundary", {{0, "foo"}, {10, "foo"}}, {"foo", "foo"});
    testDedup("chain", {{0, "foo"}, {8, "foo"}, {16, "foo"}}, {});
    testDedup("same-time", {{3, "foo"}, {3, "foo"}, {20, "bar"}}, {"bar"});
}

// follow-up: (1) memory grows forever -> logs in order only: keep just the last printed
//   time per message (LC 359); out of order: drop times older than maxSeen - lateness
// follow-up: (2) logs slightly out of order (bounded lateness L) -> release at
//   time + WINDOW + L and keep a min-heap by time instead of the queue
// follow-up: inclusive window (<= 10) -> change every "< WINDOW" to "<= WINDOW" and the
//   release test to time + WINDOW < now
