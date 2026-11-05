// Rate limiter (standard design class; ByteDance prep doc lists concurrency, so expect
// the thread-safe follow-up): allow(user, now) returns true if the user has fewer than
// `limit` accepted requests in (now - window, now], and then counts this one.

#include <iostream>
#include <string>
#include <deque>
#include <unordered_map>

using std::cout;
using std::string;
using std::deque;
using std::unordered_map;

// Sliding-window log: per user, a deque of accepted timestamps; drop expired ones from
// the front, then compare the size with limit.
// T: O(1) amortized, S: O(users * limit).
class RateLimiter {
    // max accepted requests per user per window
    int limit;
    // window length in time units
    int window;
    // accepted[user] = that user's accepted timestamps still inside the window
    unordered_map<string, deque<long long>> accepted;

public:
    RateLimiter(int limit, int window) : limit(limit), window(window) {}

    bool allow(const string& user, long long now) {
        deque<long long>& times = accepted[user];
        // <= : a request exactly `window` ago has left (now - window, now]
        while (!times.empty() && times.front() <= now - window) {
            times.pop_front();
        }
        if ((int)times.size() >= limit) {
            return false;
        }
        times.push_back(now);
        return true;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    RateLimiter limiter(2, 10);
    check(limiter.allow("amy", 0) && limiter.allow("amy", 1), "under-limit");
    check(!limiter.allow("amy", 9), "over-limit");
    check(limiter.allow("amy", 10), "window-slides");
    check(limiter.allow("bob", 9), "per-user");
}

// follow-up: thread-safe -> one mutex member; lock it for the whole allow(), because the
//   check (size < limit) and the record (push_back) must be atomic together
//   mutex m;
//   bool allow(const string& user, long long now) {
//       lock_guard<mutex> lock(m);
//       /* same body */
//   }
// follow-up: less contention -> shard users: S x {mutex, map}, shard = hash(user) % S
// follow-up: O(1) memory per user -> token bucket {tokens, last}: tokens = min(cap,
//   tokens + (now - last) * rate); allow and tokens -= 1 if tokens >= 1
// follow-up: distributed -> Redis sorted set per user (ZREMRANGEBYSCORE, ZCARD, ZADD)
//   in one Lua script so the check and the add stay atomic
