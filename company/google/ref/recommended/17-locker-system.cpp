// Locker System (Google, interview report): lockers 1..n. store(customer) assigns the
// smallest free locker and returns its number (the most recently assigned locker), or -1
// if none is free. A customer holds at most one locker: a second store while holding one
// returns -1 and changes nothing. retrieve(customer) frees the customer's locker and
// returns true, or false if the customer holds none.
//
// Clarify:
// - lockers numbered 1..n; customer ids are strings
// - "smallest free" -> lowest number, not the most recently freed one (not a FIFO)
// - a customer storing twice -> -1, nothing changes (stated); retrieve with no locker
//   -> false
// - n and the number of ops up to ~1e5 -> O(log n) per op; n up to 1e9 -> follow-up
// - one item per customer, any size? -> yes here; sizes are a follow-up
// Idea: a min-heap of free locker numbers plus a hash map customer -> locker. store pops
//   the smallest free locker; retrieve pushes the customer's locker back. A freed locker
//   is a plain push, so no stale heap entries ever appear.
// Pitfalls:
// - check the customer first, then pop: popping first leaks a locker on a double store
// - priority_queue is a max-heap by default: greater<int> for the smallest
// - erase the map entry on retrieve, or that customer can never store again
// - LockerScan: used has n + 1 slots (1-based); index 0 is never handed out
// Tests to write: freed lockers come back smallest first; full -> -1, free one, reuse;
//   double store and double retrieve; retrieve by an unknown customer.

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

// follow-up (sizes): locker i has size sizeOf[i - 1], 0 = S, 1 = M, 2 = L; an item of
// size s fits a locker of size >= s. store takes the smallest size >= s that has a free
// locker (best fit, so big lockers stay free for big items), the lowest number in it.
// One min-heap of free lockers per size. T: O(log n) per op, S: O(n).
// Clarify: item too big for every free locker -> -1; best fit rather than the lowest
//   number overall (ask; for lowest overall compare the tops of every size >= s).
// Pitfalls: retrieve pushes the locker back into the heap of its own size, not the
//   item's size, so keep sizeOf; check the customer before popping.
// Tests to write: S full spills to M, then to L; a returned L locker serves a later S;
//   an item larger than every locker -> -1.
class LockerSized {
    // freeBySize[s] = free lockers of size s (S, M, L), smallest number on top
    vector<priority_queue<int, vector<int>, greater<int>>> freeBySize;
    // sizeOf[locker - 1] = size of locker
    vector<int> sizeOf;
    // customer -> locker they hold
    unordered_map<string, int> lockerOf;

public:
    LockerSized(const vector<int>& sizes) : freeBySize(3), sizeOf(sizes) {
        for (int locker = 1; locker <= (int)sizes.size(); locker++) {
            freeBySize[sizes[locker - 1]].push(locker);
        }
    }

    int store(const string& customer, int itemSize) {
        if (lockerOf.count(customer)) { return -1; }
        for (int size = itemSize; size < (int)freeBySize.size(); size++) {
            if (freeBySize[size].empty()) { continue; }
            int locker = freeBySize[size].top();
            freeBySize[size].pop();
            lockerOf[customer] = locker;
            return locker;
        }
        return -1;
    }

    bool retrieve(const string& customer) {
        auto it = lockerOf.find(customer);
        if (it == lockerOf.end()) { return false; }
        int locker = it->second;
        freeBySize[sizeOf[locker - 1]].push(locker);
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

struct SizedOp {
    string op;
    string customer;
    // item size for a store, unused for a retrieve
    int itemSize;
};

// a retrieve's result is 1 / 0
void testSized(const string& label, const vector<int>& sizes, const vector<SizedOp>& ops,
               const vector<int>& want) {
    LockerSized lockers(sizes);
    vector<int> got;
    for (const SizedOp& op : ops) {
        if (op.op == "store") {
            got.push_back(lockers.store(op.customer, op.itemSize));
        } else {
            got.push_back(lockers.retrieve(op.customer));
        }
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
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

    testSized("sized-spill-up", {2, 0, 1},
              {{"store", "amy", 0}, {"store", "bob", 0}, {"store", "cal", 1},
               {"store", "dan", 0}},
              {2, 3, 1, -1});
    testSized("sized-own-heap", {0, 2},
              {{"store", "amy", 2}, {"retrieve", "amy", 0}, {"store", "bob", 0},
               {"store", "cal", 0}},
              {2, 1, 1, 2});
    testSized("sized-too-big", {0, 1},
              {{"store", "amy", 2}, {"store", "amy", 1}, {"store", "amy", 0}},
              {-1, 2, -1});
}

// follow-up: n is huge (10^9) -> don't pre-fill the heap: keep nextNew (the lowest
//   locker never handed out) and push only returned lockers. A returned locker is always
//   below nextNew, so the heap top wins whenever the heap is not empty:
//     if (!freeLockers.empty()) { locker = freeLockers.top(); freeLockers.pop(); }
//     else if (nextNew <= n) { locker = nextNew; nextNew++; }
//     else { return -1; }
// follow-up: locker sizes (S / M / L), an item fits its size or larger -> LockerSized
//   above
// follow-up: lastAssigned() query -> keep an int member set on each successful store
// follow-up: retrieve by locker number -> also keep vector<string> customerAt(n + 1):
//   set on store; retrieve(locker) reads it, erases lockerOf[customer], pushes locker
// follow-up: many threads -> one mutex member; lock_guard<mutex> at the top of store
//   and retrieve (the check and the pop must be one atomic step)
