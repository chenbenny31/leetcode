// LC 362 Design Hit Counter: hit(timestamp) records a hit; getHits(timestamp) counts
// hits in the past 300 seconds, i.e. in (timestamp - 300, timestamp]. Timestamps
// (seconds) never decrease.
// Follow-up: many hits per second -> memory independent of the hit count.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::queue;
using std::string;
using std::vector;

class HitCounter {
    queue<int> hits;   // timestamps of hits still in the window, oldest at front

    // drop hits that are 300 or more seconds old
    void evict(int timestamp) {
        while (!hits.empty() && hits.front() <= timestamp - 300) { hits.pop(); }
    }

public:
    // T: O(1)
    void hit(int timestamp) {
        hits.push(timestamp);
    }

    // queue + lazy eviction. T: O(1) amortized, S: O(hits in window).
    int getHits(int timestamp) {
        evict(timestamp);
        return hits.size();
    }
};

// follow-up: one bucket per second of the window, reused circularly.
// T: O(1) hit, O(300) getHits; S: O(300).
class HitCounterBuckets {
    vector<int> times;    // times[i] = the second that bucket i currently counts
    vector<int> counts;   // counts[i] = hits during second times[i]

public:
    HitCounterBuckets() : times(300, 0), counts(300, 0) {}

    void hit(int timestamp) {
        int i = timestamp % 300;
        // the bucket still holds an older second: reset it
        if (times[i] != timestamp) {
            times[i] = timestamp;
            counts[i] = 0;
        }
        counts[i]++;
    }

    int getHits(int timestamp) {
        int total = 0;
        for (int i = 0; i < 300; i++) {
            if (timestamp - times[i] < 300) { total += counts[i]; }
        }
        return total;
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    HitCounter c;
    c.hit(1);
    c.hit(1);
    c.hit(5);
    check(c.getHits(6) == 3, "same-second");
    check(c.getHits(300) == 3, "edge-300");
    check(c.getHits(301) == 1, "expire-ones");
    check(c.getHits(1000) == 0, "all-expired");
}
