// ByteDance (FastPrep, onsite, 2 sightings): operations on an integer multiset:
//   [1, x] add one x;   [2, x] remove one x (no-op if absent);   [3] query the median.
// Each query returns the median as a reduced fraction "num/den" ("3/1", "-5/2"), or
// "null" when empty.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <unordered_map>
#include <set>
#include <iterator>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::unordered_map;
using std::to_string;
using std::multiset;
using std::prev;

// odd count: median = lowTop; even: (lowTop + highTop) / 2, as a reduced fraction.
string medianFraction(bool odd, long long lowTop, long long highTop) {
    if (odd) {
        return to_string(lowTop) + "/1";
    }
    long long sum = lowTop + highTop;
    if (sum % 2 == 0) {
        return to_string(sum / 2) + "/1";
    }
    return to_string(sum) + "/2";
}

// Two heaps split the values in halves; a heap can't delete from the middle, so a
// removal is recorded in delayed and popped once it reaches a top (lazy deletion).
// T: O(log n) amortized per op, S: O(n).
class DualHeap {
    // smaller half, max on top (may still hold deleted values below the top)
    priority_queue<long long> low;
    // larger half, min on top
    priority_queue<long long, vector<long long>, greater<long long>> high;
    // delayed[x] = deletions of x not yet popped from a heap
    unordered_map<long long, int> delayed;
    // count[x] = copies of x currently in the multiset
    unordered_map<long long, int> count;
    // live elements per half; lowSize is highSize or highSize + 1
    int lowSize = 0;
    int highSize = 0;

    // pop deleted values off low's top so its top is always a live element
    void pruneLow() {
        while (!low.empty() && delayed[low.top()] > 0) {
            delayed[low.top()]--;
            low.pop();
        }
    }

    // same for high
    void pruneHigh() {
        while (!high.empty() && delayed[high.top()] > 0) {
            delayed[high.top()]--;
            high.pop();
        }
    }

    // move one element across so that lowSize - highSize is 0 or 1
    void balance() {
        if (lowSize > highSize + 1) {
            high.push(low.top());
            low.pop();
            lowSize--;
            highSize++;
            pruneLow();
        } else if (lowSize < highSize) {
            low.push(high.top());
            high.pop();
            highSize--;
            lowSize++;
            pruneHigh();
        }
    }

public:
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

    void remove(long long x) {
        if (count[x] == 0) {
            return;
        }
        count[x]--;
        delayed[x]++;
        // every live copy of x is in low exactly when x <= low.top()
        if (x <= low.top()) {
            lowSize--;
            pruneLow();
        } else {
            highSize--;
            pruneHigh();
        }
        balance();
    }

    string median() {
        if (lowSize == 0) {
            return "null";
        }
        if (lowSize > highSize) {
            return medianFraction(true, low.top(), 0);
        }
        return medianFraction(false, low.top(), high.top());
    }
};

// Follow-up, simpler: two multisets erase one copy of x directly, no lazy deletion.
// T: O(log n) per op, S: O(n).
class MedianSets {
    // smaller half; its max is *low.rbegin()
    multiset<long long> low;
    // larger half; its min is *high.begin(); low.size() is high.size() or + 1
    multiset<long long> high;

    void balance() {
        if (low.size() > high.size() + 1) {
            high.insert(*low.rbegin());
            low.erase(prev(low.end()));
        } else if (low.size() < high.size()) {
            low.insert(*high.begin());
            high.erase(high.begin());
        }
    }

public:
    void add(long long x) {
        if (low.empty() || x <= *low.rbegin()) {
            low.insert(x);
        } else {
            high.insert(x);
        }
        balance();
    }

    void remove(long long x) {
        // erase(iterator) removes one copy; erase(value) would remove all of them
        auto it = low.find(x);
        if (it != low.end()) {
            low.erase(it);
        } else {
            it = high.find(x);
            if (it == high.end()) {
                return;
            }
            high.erase(it);
        }
        balance();
    }

    string median() {
        if (low.empty()) {
            return "null";
        }
        if (low.size() > high.size()) {
            return medianFraction(true, *low.rbegin(), 0);
        }
        return medianFraction(false, *low.rbegin(), *high.begin());
    }
};

// Run the operations on DualHeap. T: O(n log n) for n operations, S: O(n).
vector<string> dynamicMedians(const vector<vector<int>>& operations) {
    DualHeap heaps;
    vector<string> result;
    for (const vector<int>& op : operations) {
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

// Same driver over the multiset version.
vector<string> dynamicMediansSets(const vector<vector<int>>& operations) {
    MedianSets sets;
    vector<string> result;
    for (const vector<int>& op : operations) {
        if (op[0] == 1) {
            sets.add(op[1]);
        } else if (op[0] == 2) {
            sets.remove(op[1]);
        } else {
            result.push_back(sets.median());
        }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& operations,
          const vector<string>& want) {
    bool ok = dynamicMedians(operations) == want;
    bool okSets = dynamicMediansSets(operations) == want;
    cout << label << (ok && okSets ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd-even", {{1, 4}, {3}, {1, 7}, {3}, {1, 2}, {3}}, {"4/1", "11/2", "4/1"});
    test("delete-missing", {{1, 3}, {2, 5}, {3}, {2, 3}, {3}}, {"3/1", "null"});
    test("negatives", {{1, -1}, {1, -4}, {3}, {1, -4}, {2, -1}, {3}}, {"-5/2", "-4/1"});
    test("dups", {{1, 2}, {1, 2}, {1, 2}, {2, 2}, {3}}, {"2/1"});
}

// follow-up: simpler than lazy heaps -> see MedianSets above; two multisets, erase one
//   copy via find(), O(log n)
// follow-up: small value range R -> Fenwick tree over counts, k-th via binary lifting,
//   O(log R) per op
// follow-up: sliding window median (LC 480) -> same DualHeap: add incoming, remove
//   outgoing, median per window, O(n log n)
// follow-up: memory-limited stream -> approximate quantiles (t-digest / KLL sketch)
