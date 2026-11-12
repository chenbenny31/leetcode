// Locker System (Google, interview report): lockers 1..n. store(customer) assigns the
// smallest free locker and returns its number (the most recently assigned locker), or -1
// if none is free. A customer holds at most one locker: a second store while holding one
// returns -1 and changes nothing. retrieve(customer) frees the customer's locker and
// returns true, or false if the customer holds none.

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <utility>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::unordered_map;
using std::pair;
using std::greater;

// straightforward: scan for the first free locker. T: store O(n), retrieve O(1), S: O(n).
class LockerScan {
    // used[locker] = locker is taken, index 0 unused
    vector<bool> used;
    // customer -> locker they hold
    unordered_map<string, int> lockerOf;

public:
    LockerScan(int n) : used(n + 1, false) {}

    int store(const string& customer) {
        if (lockerOf.count(customer)) { return -1; }
        int n = used.size();
        for (int locker = 1; locker < n; locker++) {
            if (!used[locker]) {
                used[locker] = true;
                lockerOf[customer] = locker;
                return locker;
            }
        }
        return -1;
    }

    bool retrieve(const string& customer) {
        auto it = lockerOf.find(customer);
        if (it == lockerOf.end()) { return false; }
        used[it->second] = false;
        lockerOf.erase(it);
        return true;
    }
};

// improved: min-heap of free lockers. T: O(log n) per op (build O(n log n)), S: O(n).
class Locker {
    // free locker numbers, smallest on top
    priority_queue<int, vector<int>, greater<int>> freeLockers;
    // customer -> locker they hold
    unordered_map<string, int> lockerOf;

public:
    Locker(int n) {
        for (int locker = 1; locker <= n; locker++) { freeLockers.push(locker); }
    }

    int store(const string& customer) {
        if (lockerOf.count(customer) || freeLockers.empty()) { return -1; }
        int locker = freeLockers.top();
        freeLockers.pop();
        lockerOf[customer] = locker;
        return locker;
    }

    bool retrieve(const string& customer) {
        auto it = lockerOf.find(customer);
        if (it == lockerOf.end()) { return false; }
        freeLockers.push(it->second);
        lockerOf.erase(it);
        return true;
    }
};

// ops: {"store", customer} or {"retrieve", customer}; a retrieve's result is 1 / 0
void test(const string& label, int n, const vector<pair<string, string>>& ops,
          const vector<int>& want) {
    LockerScan scan(n);
    Locker fast(n);
    vector<int> gotScan;
    vector<int> gotFast;
    for (const auto& [op, customer] : ops) {
        if (op == "store") {
            gotScan.push_back(scan.store(customer));
            gotFast.push_back(fast.store(customer));
        } else {
            gotScan.push_back(scan.retrieve(customer));
            gotFast.push_back(fast.retrieve(customer));
        }
    }
    bool ok = gotScan == want && gotFast == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("smallest-free", 3,
         {{"store", "amy"}, {"store", "bob"}, {"store", "cal"}, {"retrieve", "bob"},
          {"retrieve", "amy"}, {"store", "dan"}, {"store", "eve"}},
         {1, 2, 3, 1, 1, 1, 2});
    test("full-reuse", 1,
         {{"store", "amy"}, {"store", "bob"}, {"retrieve", "amy"}, {"store", "bob"}},
         {1, -1, 1, 1});
    test("double-store", 2,
         {{"store", "amy"}, {"store", "amy"}, {"retrieve", "amy"}, {"retrieve", "amy"}},
         {1, -1, 1, 0});
    test("unknown", 1, {{"retrieve", "amy"}}, {0});
}

// follow-up: n is huge (10^9) -> don't pre-fill the heap: keep nextNew (lowest never-used
//   locker) plus a heap of returned lockers; take min(heap top, nextNew)
// follow-up: locker sizes (S / M / L), an item fits its size or larger -> one free heap
//   per size; try the item's size first, then the next larger sizes
// follow-up: lastAssigned() query -> keep an int member set on each successful store
// follow-up: retrieve by locker number -> also keep a vector customerAt[locker]
