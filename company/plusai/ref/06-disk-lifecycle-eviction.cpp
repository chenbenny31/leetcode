// PlusAI (JD): Disk Lifecycle Eviction (Medium). Pattern: min-heap + running total.
// A vehicle disk holds `capacity` bytes. Recordings (id, size, priority) arrive in time
// order; after each arrival, while total > capacity evict the lowest priority (ties:
// oldest arrival). A recording bigger than capacity is evicted right away, touching no
// other. Return the evicted ids in order.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::greater;

struct Recording {
    int id;
    long long size;
    int priority;
};

// This is a min-heap keyed by (priority, arrival index) plus a running byte total; the
// pair order gives the oldest-first tie rule. T: O(n log n), S: O(n).
vector<int> evictions(long long capacity, const vector<Recording>& recs) {
    // (priority, arrival index) of each recording on disk; top = next to evict
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    long long total = 0;
    vector<int> evicted;
    int n = recs.size();
    for (int i = 0; i < n; i++) {
        // can never fit: drop it alone instead of evicting everything else first
        if (recs[i].size > capacity) {
            evicted.push_back(recs[i].id);
            continue;
        }
        heap.push({recs[i].priority, i});
        total += recs[i].size;
        while (total > capacity) {
            int victim = heap.top().second;
            heap.pop();
            total -= recs[victim].size;
            evicted.push_back(recs[victim].id);
        }
    }
    return evicted;
}

void test(const string& label, long long capacity, const vector<Recording>& recs,
          const vector<int>& want) {
    cout << label << (evictions(capacity, recs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lowest-priority", 10, {{1, 4, 5}, {2, 4, 1}, {3, 4, 3}}, {2});
    test("tie-oldest", 10, {{1, 6, 2}, {2, 3, 2}, {3, 3, 2}}, {1});
    test("cascade", 10, {{1, 3, 1}, {2, 3, 2}, {3, 9, 5}}, {1, 2});
    test("too-big", 10, {{1, 4, 1}, {2, 20, 9}, {3, 4, 2}}, {2});
}

// follow-up: recordings get pinned / deleted / re-prioritized -> set<tuple<priority,
//   arrival, id>> plus id -> its key, so erase(key) and re-insert are O(log n)
// follow-up: only evict recordings already uploaded -> skip (or park) non-uploaded ones
