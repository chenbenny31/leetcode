// Thread-safe sliding-window rate limiter (ByteDance prep doc: concurrency correctness):
// allow(user, now) returns true if the user has fewer than `limit` accepted requests in
// (now - window, now], and then counts this one. Many threads call allow at once.
// Follow-up: lock striping (one mutex per shard of users) to cut contention.

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::deque;
using std::thread;
using std::mutex;
using std::lock_guard;
using std::atomic;
using std::ref;
using std::unordered_map;

// Per-user log of accepted timestamps. The check ("fewer than limit?") and the update
// ("record it") must happen under the same lock, or two threads could both see room
// for the last slot. T: O(1) amortized per call, S: O(users * limit).
class RateLimiter {
    int limit;
    int window;
    // accepted[user] = that user's accepted timestamps still inside the window
    unordered_map<string, deque<long long>> accepted;
    // guards accepted
    mutex m;

public:
    RateLimiter(int limit, int window) : limit(limit), window(window) {}

    bool allow(const string& user, long long now) {
        lock_guard<mutex> lock(m);
        deque<long long>& times = accepted[user];
        while (!times.empty() && times.front() <= now - window) { times.pop_front(); }
        if ((int)times.size() >= limit) { return false; }
        times.push_back(now);
        return true;
    }
};

// fire `calls` requests at the same instant and count how many got through
void burst(RateLimiter& limiter, const string& user, int calls, atomic<int>& granted) {
    for (int i = 0; i < calls; i++) {
        if (limiter.allow(user, 0)) { granted++; }
    }
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    RateLimiter seq(2, 10);
    bool a = seq.allow("amy", 0) && seq.allow("amy", 1);
    bool b = !seq.allow("amy", 9);
    bool c = seq.allow("amy", 10) && seq.allow("bob", 9);
    check(a && b && c, "window-slides-per-user");

    RateLimiter shared(100, 60);
    atomic<int> granted{0};
    vector<thread> threads;
    for (int t = 0; t < 8; t++) {
        threads.push_back(thread(burst, ref(shared), string("amy"), 50, ref(granted)));
    }
    for (thread& t : threads) { t.join(); }
    check(granted == 100, "eight-threads-exactly-limit");
}
