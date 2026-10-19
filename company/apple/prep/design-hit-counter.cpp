// hit(timestamp) records a hit; getHits(timestamp) return hits in past 300 sec
// timestamps are non-decre

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// queue of hit-timestamps, lazy evict from front, T: O(n), S: O(n)
class HitCounter {
private:
    queue<int> hits;

    void evict(int timestamp) {
        while (!hits.empty() && hits.front() <= timestamp - 300) {
            hits.pop();
        }
    }

public:
    // T: O(1) amortized
    void hit(int timestamp) {
        hits.push(timestamp);
        evict(timestamp);
    }

    // T: O(1) amortized, each hit is popped once
    int getHits(int timestamp) {
        evict(timestamp);
        return hits.size();
    }
};

// one bucket for window, T: O(1), S: O(1)
class HitCounterBuckets {
private:
    vector<int> times; // time[i] = bucket[i]'s timestamp
    vector<int> counts; // counts[i] = buckets[i]'s hits

public:
    HitCounterBuckets() : times(300, 0), counts(300, 0) {}

    void hit(int timestamp) {
        int i = timestamp % 300;
        if (times[i] != timestamp) { // overwrite both
            times[i] = timestamp;
            counts[i] = 0;
        }
        counts[i]++;
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

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    HitCounter q;
    HitCounterBuckets b;
    for (int t : {1, 2, 3}) {
        q.hit(t);
        b.hit(t);
    }
    check(q.getHits(4) == 3 && b.getHits(4) == 3, "in-window");
    q.hit(300);
    b.hit(300);
    check(q.getHits(300) == 4 && b.getHits(300) == 4, "edge");
    check(q.getHits(301) == 3 && b.getHits(301) == 3, "expire");
    check(q.getHits(1000) == 0 && b.getHits(1000) == 0, "all-gone");
}
