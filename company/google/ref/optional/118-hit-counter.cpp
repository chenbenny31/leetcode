// 118 · Design Hit Counter — LC 362
// Mother: select/14-design/03-hit-counter.cpp
// hit(timestamp) records a hit; getHits(timestamp) = num hits in last 300 s
// window = (timestamp - 300, timestamp]; timestamps in seconds
// reported: five-minute hit counter x2
//
// clarify:
// window edges -> (timestamp - 300, timestamp]
// timestamps -> non-decreasing
// many hits in same second -> solution 2 (buckets)
// getHits w/ no hits -> 0

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// solution 1: queue of hit times; simplest, memory grows w/ hits in window
// core idea: every call pops the fronts that are 300+ s old
// T: O(1) per op, S: O(hits in window) — amortized, each hit popped once
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

// solution 2: 300 circular buckets; use it when many hits per second
// core idea: bucket = (second, cnt); a bucket holding an older second is reset
// getHits sums the buckets still in the window
// T: hit O(1), getHits O(300), S: O(300)
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

// follow-up:
// concurrent hits -> mutex per counter, or per-bucket atomics (reset needs lock / CAS)
// out-of-order timestamps -> buckets still work while delay < window
