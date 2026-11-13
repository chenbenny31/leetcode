// LC 362 Design Hit Counter (Medium) — sliding window queue
// hit(timestamp) records a hit; getHits(timestamp) = hits in (timestamp - 300,
// timestamp]. Timestamps are seconds and never decrease.
// Clarify: the hit exactly 300 seconds back is out; many hits in one second;
//   timestamps monotonic across hit and getHits; concurrency.

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// Core idea: queue of hit times; pop everything 300 or more seconds old.
// T: O(1) amortized per op (each hit is popped once), S: O(hits in window).
class HitCounter {
    // timestamps of hits still inside the window, oldest at the front
    queue<int> hits;

    void evict(int timestamp) {
        while (!hits.empty() && hits.front() <= timestamp - 300) {
            hits.pop();
        }
    }

public:
    void hit(int timestamp) {
        hits.push(timestamp);
        evict(timestamp);
    }

    int getHits(int timestamp) {
        evict(timestamp);
        return hits.size();
    }
};

void test(const string& label, const vector<int>& hits, int now, int want) {
    HitCounter counter;
    for (int time : hits) {
        counter.hit(time);
    }
    cout << label << (counter.getHits(now) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("same-second", {5, 5, 5}, 5, 3);
    test("edge-300", {1, 2, 3, 300}, 300, 4);
    test("first-expires", {1, 2, 3, 300}, 301, 3);
    test("all-expired", {1, 2}, 1000, 0);
    return 0;
}

// follow-up (ideas only):
// - many hits per second -> 300 circular buckets (second, count) at timestamp % 300;
//   reset a bucket that holds an older second; getHits sums the fresh ones, O(300)
// - concurrent hits -> mutex per counter, or per-bucket atomics in the bucket version
//   (the reset race needs a lock or CAS on the bucket's second)
// - out-of-order timestamps -> buckets still work as long as the delay < window
