// Logger Dedup, two parts (Google, interview report). Window rule for both parts: two
// copies of a message are duplicates iff |t1 - t2| < 10 (exactly 10 apart is fine).
// (1) Out-of-order rate limiter: (timestamp, message) arrive in any time order. Print
//     a message iff no PRINTED copy is within the window; only printed copies are
//     recorded.
// (2) Delayed duplicate suppression: logs arrive sorted by time. Output a log iff no
//     other log with the same message is within the window, before or after it. Output
//     order = time order.
//
// Clarify (1):
// - window -> |t1 - t2| < 10 (stated): copies exactly 10 apart both print
// - does a suppressed copy count? -> no, only printed copies block later ones
// - timestamps in any order, repeats allowed (the same time twice: second is blocked)
// - up to ~1e5 calls -> O(log k) per call (k = printed copies of that message)
// Idea (1): per message an ordered set of printed times; only the nearest printed time on
//   each side of the new timestamp can be within the window, so lower_bound and the
//   element before it decide.
// Pitfalls (1):
// - check both sides: logs arrive out of order, so a printed time can be after this one
// - prev(it) only when it != begin(); insert only when printing
// - LC 359's "last printed time per message" is wrong here: a late log needs every
//   printed time around it, not only the newest
// Tests to write (1): exactly 10 apart prints; a blocked copy is not recorded; out of
//   order with blocks from the later and the earlier side.
// Clarify (2):
// - a log is dropped if any other copy is within the window, even a dropped one (0, 8,
//   16: all three dropped)
// - same timestamp twice -> duplicates (difference 0); output = kept messages in time
//   order; empty input -> empty output
// - streaming: when may a log be output? -> once time + WINDOW has arrived (no later
//   copy can come within the window), or at the end of the stream
// Idea (2): only the nearest earlier and the nearest later copy matter. Offline: a
//   forward and a backward pass with "last time seen" per message. Streaming: hold each
//   log in a time-ordered queue until time + WINDOW; a later copy within the window
//   exists iff the log is no longer its message's latest pending copy.
// Pitfalls (2):
// - compare with the last time seen (the nearest), not the first occurrence; clear the
//   map between the passes
// - streaming: release(timestamp) before recording the arrival; then a copy that replaces
//   a pending log as latest is always within that log's window
// - finish() flushes the last window, or the tail of the stream is never output
// Tests to write (2): both copies dropped; exactly 10 apart both kept; a chain drops all;
//   same timestamp.

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <unordered_map>
#include <utility>
#include <iterator>
#include <functional>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::set;
using std::queue;
using std::unordered_map;
using std::pair;
using std::prev;
using std::priority_queue;
using std::greater;
using std::max;

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

// follow-up (2): logs arrive up to L late (each timestamp >= the largest one seen so far
// - L). A min-heap puts them back in order: a log with time <= maxSeen - L can no longer
// be preceded by a new arrival, so it moves on into Dedup, which then sees sorted input.
// Output is delayed by up to L + WINDOW. T: O(log b) per log, b = logs within L of the
// newest; S: O(b + logs within one window).
// Pitfalls: finish() drains the heap into dedup before dedup.finish(); copy top() before
//   pop(). Tests to write: a late copy drops both; reordered output; L = 0 like Dedup.
class DedupLate {
    // logs not yet passed on, earliest (time, message) on top
    priority_queue<pair<int, string>, vector<pair<int, string>>,
                   greater<pair<int, string>>> early;
    // gets the logs in time order
    Dedup dedup;
    // max lateness L
    int lateness;
    // largest timestamp seen so far
    int maxSeen = INT_MIN;

    // pass every log with time <= upTo to dedup, earliest first; returns what it releases
    vector<string> forward(int upTo) {
        vector<string> out;
        while (!early.empty() && early.top().first <= upTo) {
            auto [time, message] = early.top();
            early.pop();
            for (const string& log : dedup.push(time, message)) { out.push_back(log); }
        }
        return out;
    }

public:
    explicit DedupLate(int maxLateness) : lateness(maxLateness) {}

    vector<string> push(int timestamp, const string& message) {
        early.push({timestamp, message});
        maxSeen = max(maxSeen, timestamp);
        return forward(maxSeen - lateness);
    }

    vector<string> finish() {
        vector<string> out = forward(INT_MAX);
        for (const string& log : dedup.finish()) { out.push_back(log); }
        return out;
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

void testLate(const string& label, const vector<pair<int, string>>& arrivals,
              int lateness, const vector<string>& want) {
    DedupLate dedup(lateness);
    vector<string> streamed;
    for (const auto& [timestamp, message] : arrivals) {
        for (const string& out : dedup.push(timestamp, message)) {
            streamed.push_back(out);
        }
    }
    for (const string& out : dedup.finish()) { streamed.push_back(out); }
    cout << label << (streamed == want ? " PASS\n" : " FAIL\n");
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

    testLate("late-both-dropped", {{5, "foo"}, {0, "foo"}, {20, "bar"}}, 5, {"bar"});
    testLate("late-reordered", {{2, "foo"}, {0, "bar"}, {13, "foo"}, {11, "bar"}}, 3,
             {"bar", "foo", "bar", "foo"});
    testLate("late-zero", {{0, "foo"}, {10, "foo"}, {12, "bar"}}, 0,
             {"foo", "foo", "bar"});
}

// follow-up: (1) logs in time order (LC 359) -> one last printed time per message:
//     unordered_map<string, int> lastPrinted;
//     auto it = lastPrinted.find(message);
//     if (it != lastPrinted.end() && timestamp - it->second < WINDOW) { return false; }
//     lastPrinted[message] = timestamp;
//     return true;
// follow-up: (1) memory grows forever, lateness at most L -> a printed time p can still
//   block only if p > maxSeen - L - WINDOW; keep set<pair<int, string>> byTime of all
//   printed (time, message) and after each call:
//     while (!byTime.empty() && byTime.begin()->first <= maxSeen - L - WINDOW) {
//         printed[byTime.begin()->second].erase(byTime.begin()->first);
//         byTime.erase(byTime.begin());
//     }
// follow-up: (2) logs up to L late -> DedupLate above
// follow-up: inclusive window (<= 10) -> change every "< WINDOW" to "<= WINDOW", the
//   release test to time + WINDOW < now, and finish() to release(back.time + WINDOW + 1)
