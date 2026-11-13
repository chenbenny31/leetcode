// LC 362 Design Hit Counter (Medium) — sliding window queue
// hit(timestamp) records a hit; getHits(timestamp) = hits in (timestamp - 300, timestamp]
// timestamps are seconds, never decrease
//
// clarify:
// hit exactly 300 s back -> out of the window
// many hits in one second -> one queue entry each; buckets -> follow-up
// timestamps monotonic across hit and getHits -> evict from the front only
// concurrency -> single-threaded here; follow-up

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// core idea: queue of hit times; pop everything >= 300 s old
// T: O(1) amortized per op, S: O(hits in window) — each hit is popped once
class HitCounter {
    // timestamps of hits still in the window, oldest at front
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

// follow-up:
// many hits per second -> 300 circular buckets (second, cnt) at timestamp % 300, O(300)
// concurrent hits -> mutex per counter, or per-bucket atomics (reset needs lock / CAS)
// out-of-order timestamps -> buckets still work while delay < window
