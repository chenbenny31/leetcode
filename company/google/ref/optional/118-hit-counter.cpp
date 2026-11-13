// 118 · Design Hit Counter — LC 362
// Mother problem: select/14-design/03-hit-counter.cpp
//
// Google reports: five-minute hit counter ×2
//
// Clarify
// - window? → hits in (t − 300, t], seconds
// - timestamps? → non-decreasing
// - many hits per second? → the bucket version keeps O(300) memory
// - getHits with no hits → 0
//
// Core idea
// - `HitCounter`: queue of hit times; evict fronts ≤ t − 300 on every call. O(1)
//   amortized, S O(hits in window).
// - `HitCounterBuckets`: 300 circular buckets of (second, count); reset a bucket when it
//   holds an older second; getHits sums buckets with t − time < 300. O(1) hit, O(300)
//   getHits.
//
// Tests: same second, edge 300, first expires, all expired
//
// Follow-ups
// - many hits per second → `HitCounterBuckets` below
// - concurrent hits → mutex per counter, or per-bucket atomics (bucket reset needs a
//   lock or CAS)
// - out-of-order timestamps → buckets still work while the delay is < window
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// Queue of hit times; pop everything 300+ seconds old.
// T: O(1) amortized per op (each hit popped once), S: O(hits in window).
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

// Follow-up (many hits per second): one bucket per second of the window, reused
// circularly. T: O(1) hit, O(300) getHits; S: O(300).
class HitCounterBuckets {
    // times[i] = the second bucket i currently counts
    vector<int> times;
    // counts[i] = hits during second times[i]
    vector<int> counts;

public:
    HitCounterBuckets() : times(300, 0), counts(300, 0) {}

    void hit(int timestamp) {
        int bucket = timestamp % 300;
        // bucket still holds an older second: reset it
        if (times[bucket] != timestamp) {
            times[bucket] = timestamp;
            counts[bucket] = 0;
        }
        counts[bucket]++;
    }

    int getHits(int timestamp) {
        int total = 0;
        for (int i = 0; i < 300; i++) {
            if (timestamp - times[i] < 300) {
                total += counts[i];
            }
        }
        return total;
    }
};

void test(const string& label, const vector<int>& hits, int now, int want) {
    HitCounter queueCounter;
    HitCounterBuckets bucketCounter;
    for (int time : hits) {
        queueCounter.hit(time);
        bucketCounter.hit(time);
    }
    bool ok = queueCounter.getHits(now) == want && bucketCounter.getHits(now) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("same-second", {5, 5, 5}, 5, 3);
    test("edge-300", {1, 2, 3, 300}, 300, 4);
    test("first-expires", {1, 2, 3, 300}, 301, 3);
    test("all-expired", {1, 2}, 1000, 0);
}
