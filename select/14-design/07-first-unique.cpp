// LC 1429 First Unique Number (Medium). Pattern: queue + count, lazy cleanup.
// FirstUnique(nums) then add(value); showFirstUnique() returns the oldest value that has
// appeared exactly once so far, or -1. (Amazon "oldest one-time visitor" family.)

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;

// Every value enters the queue once, in arrival order; the front is dropped lazily once
// it is seen again. T: O(1) amortized per op, S: O(distinct values).
class FirstUnique {
    // values in first-arrival order; the front may be stale (count > 1)
    queue<int> order;
    // count[v] = times v has been added
    unordered_map<int, int> count;

public:
    FirstUnique(const vector<int>& nums) {
        for (int num : nums) { add(num); }
    }

    void add(int value) {
        count[value]++;
        if (count[value] == 1) { order.push(value); }
    }

    int showFirstUnique() {
        // each value is popped at most once, so this is O(1) amortized
        while (!order.empty() && count[order.front()] > 1) {
            order.pop();
        }
        if (order.empty()) { return -1; }
        return order.front();
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    FirstUnique firstUnique({2, 3, 5});
    check(firstUnique.showFirstUnique() == 2, "oldest-first");
    firstUnique.add(2);
    check(firstUnique.showFirstUnique() == 3, "repeat-drops-out");
    firstUnique.add(3);
    firstUnique.add(5);
    check(firstUnique.showFirstUnique() == -1, "none-left");
    firstUnique.add(7);
    check(firstUnique.showFirstUnique() == 7, "new-unique");
}

// follow-up: strict O(1) per op -> list<int> of uniques + map value -> list iterator;
//   erase the node when a value repeats (same shape as LRU)
// follow-up: "unique within the last k events" -> also expire from the front by time
