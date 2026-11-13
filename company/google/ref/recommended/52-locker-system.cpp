// Locker System (Google, interview report)
// (1) lockers 1..n; store(c) = smallest free num, -1 if full / has one; retrieve(c) frees
// (2) lockers / items have size S, M, L; an item fits its size or larger
//
// clarify:
// "smallest free" -> lowest num, not most recently freed
// double store -> -1, nothing changes
// retrieve w/o a locker -> false
// n and num of ops -> scan vs heap
// item sizes -> follow-up (2)

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

// (1) solution 1: scan; baseline
// core idea: scan for the first free locker
// T: store O(n), retrieve O(1), S: O(n)
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

// (1) solution 2: min-heap; use when n or num of ops is large
// core idea: min-heap of free nums + map customer -> locker; store pops, retrieve pushes
// T: O(log n) per op, build O(n log n), S: O(n)
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

// (2) sizes
// core idea: min-heap of free lockers per size; store takes smallest non-empty size >= s
// T: O(log n) per op, S: O(n)
class LockerSized {
    // freeBySize[s] = free lockers of size s (0 = S, 1 = M, 2 = L), smallest num on top
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

// follow-up:
// n huge (1e9) -> no pre-fill: nextNew = lowest num never handed out, heap = returned
// lowest num among sizes >= s (not best fit) -> compare those heap tops
// lastAssigned() -> int member set on each successful store
// retrieve by locker num -> also keep customerAt[locker]
// many threads -> one mutex at top of store / retrieve (check + pop must be atomic)
