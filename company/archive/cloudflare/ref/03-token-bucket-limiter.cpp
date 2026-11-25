// Cloudflare (reported): Token Bucket Rate Limiter (Medium). Pattern: map + lazy refill.
// Each client has a bucket of capacity `burst` (starts full) refilled by `rate` tokens
// per second; times are integer seconds, nondecreasing. allow(client, t): tokens =
// min(burst, tokens + (t - last) * rate), then take 1 token if any -> true, else false.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <algorithm>

using std::cout;
using std::min;
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

// This is a hash map client -> (tokens, last refill time), refilled lazily on each
// request. T: O(1) per request, S: O(clients).
class RateLimiter {
    // bucket capacity
    long long burst;
    // tokens added per second
    long long rate;
    // client -> (tokens left, time of last refill)
    unordered_map<string, pair<long long, int>> buckets;

public:
    RateLimiter(long long burstSize, long long perSecond)
        : burst(burstSize), rate(perSecond) {}

    bool allow(const string& client, int t) {
        if (buckets.count(client) == 0) {
            buckets[client] = {burst, t};
        }
        auto& [tokens, last] = buckets[client];
        // long long: (t - last) * rate can pass 2^31 after a long idle gap
        tokens = min(burst, tokens + (t - last) * rate);
        last = t;
        if (tokens == 0) { return false; }
        tokens--;
        return true;
    }
};

void test(const string& label, long long burst, long long rate,
          const vector<pair<string, int>>& requests, const vector<bool>& want) {
    RateLimiter limiter(burst, rate);
    vector<bool> got;
    for (const auto& [client, t] : requests) {
        got.push_back(limiter.allow(client, t));
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("burst-then-deny", 2, 1, {{"10.0.0.1", 0}, {"10.0.0.1", 0}, {"10.0.0.1", 0}},
         {true, true, false});
    test("refill-capped", 2, 1,
         {{"10.0.0.1", 0}, {"10.0.0.1", 0}, {"10.0.0.1", 9}, {"10.0.0.1", 9},
          {"10.0.0.1", 9}},
         {true, true, true, true, false});
    test("refill-rate", 1, 2,
         {{"10.0.0.1", 0}, {"10.0.0.1", 0}, {"10.0.0.1", 1}, {"10.0.0.1", 1}},
         {true, false, true, false});
    test("per-client", 1, 1, {{"10.0.0.1", 0}, {"10.0.0.1", 0}, {"10.0.0.2", 0}},
         {true, false, true});
}

// follow-up: millions of idle clients -> a bucket idle >= burst/rate s is full; drop it.
// follow-up: one limit across many edges -> per-edge sliding-window counters, synced:
//   estimate = prevCount * (1 - elapsed / window) + curCount.
