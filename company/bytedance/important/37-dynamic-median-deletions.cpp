// ByteDance (FastPrep, onsite, 2 sightings): process operations on an integer multiset,
// one answer per query:
//   [1, x] add one x;   [2, x] remove one x (no-op if absent);   [3] query the median.
// A non-empty median is returned as a reduced fraction "num/den" (den > 0), e.g. "3/1"
// or "-5/2"; an empty multiset returns "null".
// Report: two heaps + lazy deletion + counts of valid elements. Follow-ups (discussion
// only): small value range -> counting array / Fenwick tree; memory-limited ->
// approximate quantiles (buckets / sketches); concurrency -> locking or sharding.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::unordered_map;
using std::to_string;

// low holds the smaller half (max-heap), high the larger half (min-heap). Deleting
// from the middle of a heap is impossible, so a deleted value is only recorded in
// delayed and physically popped once it reaches a top. lowSize / highSize count the
// valid elements, and lowSize is highSize or highSize + 1.
class DualHeap {
    priority_queue<long long> low;
    priority_queue<long long, vector<long long>, greater<long long>> high;
    // delayed[x] = deletions of x not yet popped from a heap
    unordered_map<long long, int> delayed;
    // count[x] = copies of x currently in the multiset
    unordered_map<long long, int> count;
    int lowSize = 0;
    int highSize = 0;

    // pop deleted values off the top so the top is always a live element
    template <typename Heap>
    void prune(Heap& heap) {
        while (!heap.empty() && delayed[heap.top()] > 0) {
            delayed[heap.top()]--;
            heap.pop();
        }
    }

    // move one element across so that lowSize - highSize is 0 or 1
    void balance() {
        if (lowSize > highSize + 1) {
            high.push(low.top());
            low.pop();
            lowSize--;
            highSize++;
            prune(low);
        } else if (lowSize < highSize) {
            low.push(high.top());
            high.pop();
            highSize--;
            lowSize++;
            prune(high);
        }
    }

public:
    // T: O(log n)
    void add(long long x) {
        count[x]++;
        if (lowSize == 0 || x <= low.top()) {
            low.push(x);
            lowSize++;
        } else {
            high.push(x);
            highSize++;
        }
        balance();
    }

    // Every live copy of x is in low exactly when x <= low.top(). T: O(log n) amortized.
    void remove(long long x) {
        if (count[x] == 0) {
            return;
        }
        count[x]--;
        delayed[x]++;

        if (x <= low.top()) {
            lowSize--;
            prune(low);
        } else {
            highSize--;
            prune(high);
        }
        balance();
    }

    // T: O(1)
    string median() {
        if (lowSize == 0) {
            return "null";
        }
        if (lowSize > highSize) {
            return to_string(low.top()) + "/1";
        }

        // even size: (a + b) / 2, reduced when a + b is even
        long long sum = low.top() + high.top();
        if (sum % 2 == 0) {
            return to_string(sum / 2) + "/1";
        }
        return to_string(sum) + "/2";
    }
};

// T: O(n log n) for n operations, S: O(n).
vector<string> dynamicMedians(const vector<vector<int>>& operations) {
    DualHeap heaps;
    vector<string> result;
    for (const auto& op : operations) {
        if (op[0] == 1) {
            heaps.add(op[1]);
        } else if (op[0] == 2) {
            heaps.remove(op[1]);
        } else {
            result.push_back(heaps.median());
        }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& operations,
          const vector<string>& want) {
    cout << label << (dynamicMedians(operations) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd-even", {{1, 4}, {3}, {1, 7}, {3}, {1, 2}, {3}}, {"4/1", "11/2", "4/1"});
    test("delete-missing", {{1, 3}, {2, 5}, {3}, {2, 3}, {3}}, {"3/1", "null"});
    test("negatives", {{1, -1}, {1, -4}, {3}, {1, -4}, {2, -1}, {3}}, {"-5/2", "-4/1"});
    test("dups", {{1, 2}, {1, 2}, {1, 2}, {2, 2}, {3}}, {"2/1"});
}
