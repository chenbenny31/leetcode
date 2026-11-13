// Logger Dedup, three parts (Google, interview report)
// window rule for (1), (2): two copies are dups iff |t1 - t2| < 10 (exactly 10 is fine)
// (1) out-of-order rate limiter: print iff no PRINTED copy within window
// (2) delayed dup suppression, sorted input: output iff no same-message log within window
// (3) keep latest per message, sorted by (timestamp, message); stream: add + getNext
//
// clarify:
// suppressed copy still blocks? -> no, only printed copies are recorded
// same timestamp twice -> dups (diff 0 < 10)
// when does (2) emit? -> once time + WINDOW has arrived, in time order
// (3) add w/ older time -> ignored; getNext on empty -> {-1, ""}

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
using std::sort;

const int WINDOW = 10;

// (1) out-of-order rate limiter
// core idea: ordered set of printed times per message; check nearest one on each side
// T: O(log k) per call, S: O(printed logs) — k = printed copies of the message
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

// (2) offline
// core idea: only nearest earlier / later copy matter -> forward + backward pass
// T: O(n), S: O(n)
vector<string> uniqueLogs(const vector<pair<int, string>>& logs) {
    int n = logs.size();
    // keep[i] = log i has no same-message neighbor within window
    vector<bool> keep(n, true);
    // message -> time of its latest occurrence so far in the current pass
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

// (2) streaming
// core idea: hold each log until time + WINDOW has arrived
// a later copy within window exists iff log is no longer its message's latest pending
// T: O(1) per log, S: O(logs within one window) — each log queued and released once
class Dedup {
    // logs not yet released, in time order
    queue<Pending> pending;
    // message -> (time, seq) of its latest pending occurrence
    unordered_map<string, pair<int, int>> latest;
    int nextSeq = 0;

    // release logs w/ time + WINDOW <= now
    vector<string> release(int now) {
        vector<string> out;
        while (!pending.empty() && pending.front().time + WINDOW <= now) {
            Pending log = pending.front();
            pending.pop();
            // later copy exists iff this log is no longer the latest one
            if (latest[log.message].second != log.seq) { continue; }
            latest.erase(log.message);
            if (!log.dup) { out.push_back(log.message); }
        }
        return out;
    }

public:
    // returns logs released by this arrival
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

// (2) logs up to L late
// core idea: min-heap reorders; a log w/ time <= maxSeen - L is safe to pass on to Dedup
// T: O(log b) per log, S: O(b + one window) — b = logs within L of the newest
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

    // pass every log w/ time <= upTo to dedup, earliest first; returns its releases
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

// (3) batch
// core idea: latest time per message in a map, then sort survivors
// T: O(n + u log u), S: O(u) — u = distinct messages
vector<pair<int, string>> dedupKeepLatest(const vector<pair<int, string>>& logs) {
    // message -> its latest timestamp
    unordered_map<string, int> latest;
    for (const auto& [time, message] : logs) {
        auto it = latest.find(message);
        if (it == latest.end()) {
            latest[message] = time;
        } else {
            it->second = max(it->second, time);
        }
    }
    vector<pair<int, string>> out;
    for (const auto& [message, time] : latest) { out.push_back({time, message}); }
    // pair order = timestamp, then message
    sort(out.begin(), out.end());
    return out;
}

// (3) streaming
// core idea: min-heap of entries + latest time per message; stale entries skipped lazily
// T: O(log n) per add / getNext, S: O(adds not yet popped) — getNext amortized
class LatestLogStream {
    // (timestamp, message), earliest on top; may hold stale older entries
    priority_queue<pair<int, string>, vector<pair<int, string>>,
                   greater<pair<int, string>>> heap;
    // message -> latest timestamp among entries not yet returned
    unordered_map<string, int> latest;

public:
    void add(int timestamp, const string& message) {
        auto it = latest.find(message);
        if (it != latest.end() && it->second >= timestamp) { return; }
        latest[message] = timestamp;
        heap.push({timestamp, message});
    }

    pair<int, string> getNext() {
        while (!heap.empty()) {
            pair<int, string> top = heap.top();
            heap.pop();
            auto it = latest.find(top.second);
            // stale: a newer copy replaced it, or message was already returned
            if (it == latest.end() || it->second != top.first) { continue; }
            latest.erase(it);
            return top;
        }
        return {-1, ""};
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

void testLatest(const string& label, const vector<pair<int, string>>& logs,
                const vector<pair<int, string>>& want) {
    LatestLogStream stream;
    for (const auto& [timestamp, message] : logs) { stream.add(timestamp, message); }
    vector<pair<int, string>> drained;
    pair<int, string> entry = stream.getNext();
    while (entry.first != -1) {
        drained.push_back(entry);
        entry = stream.getNext();
    }
    bool ok = dedupKeepLatest(logs) == want && drained == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(bool ok, const string& label) {
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

    testLate("late-both-dropped", {{5, "foo"}, {0, "foo"}, {20, "bar"}}, 5, {"bar"});
    testLate("late-reordered", {{2, "foo"}, {0, "bar"}, {13, "foo"}, {11, "bar"}}, 3,
             {"bar", "foo", "bar", "foo"});
    testLate("late-zero", {{0, "foo"}, {10, "foo"}, {12, "bar"}}, 0,
             {"foo", "foo", "bar"});

    testLatest("keep-latest", {{1, "disk"}, {4, "cpu"}, {3, "disk"}},
               {{3, "disk"}, {4, "cpu"}});
    testLatest("older-ignored", {{5, "disk"}, {2, "disk"}, {1, "cpu"}},
               {{1, "cpu"}, {5, "disk"}});
    testLatest("tie-by-message", {{2, "disk"}, {2, "cpu"}}, {{2, "cpu"}, {2, "disk"}});
    testLatest("no-logs", {}, {});

    LatestLogStream stream;
    stream.add(5, "disk");
    stream.add(3, "cpu");
    vector<pair<int, string>> got = {stream.getNext()};
    stream.add(1, "cpu");
    stream.add(8, "disk");
    got.push_back(stream.getNext());
    got.push_back(stream.getNext());
    got.push_back(stream.getNext());
    vector<pair<int, string>> want = {{3, "cpu"}, {1, "cpu"}, {8, "disk"}, {-1, ""}};
    check(got == want, "interleaved");
}

// follow-up:
// (1) logs in time order (LC 359) -> one last printed time per message is enough
// (1) memory grows, lateness <= L -> erase printed times <= maxSeen - L - WINDOW
// inclusive window -> "< WINDOW" becomes "<= WINDOW"; release if time + WINDOW < now
// (3) unbounded stream -> watermark W = max time - lateness; emit + drop below W - TTL
