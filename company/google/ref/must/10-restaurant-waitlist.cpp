// Restaurant Waitlist (Google, interview report). join(partyId, size): the party joins
// the back of the line (ignored if that id is already waiting). leave(partyId): the
// party leaves from any position (no-op if absent). seat(tableSize): remove and return
// the earliest-joined party with size <= tableSize, or -1 if none fits. A party that
// leaves and joins again goes to the back.
//
// Clarify:
// - "fits" -> size <= tableSize; among the fitting parties the earliest joined wins, not
//   the largest (largest fit: follow-up)
// - join with an id already waiting -> ignored; leave of an absent id -> no-op
// - rejoin after leaving -> back of the line (a new arrival order)
// - sizes >= 1; nobody fits or the line is empty -> -1
// - up to ~1e5 ops, few distinct sizes S (a restaurant: <= ~20) -> one bucket per size;
//   the scan over the whole line, O(n) per op, is the brute force
// Idea: one FIFO per party size. The earliest fitting party is the front of some bucket
//   with size <= tableSize, so seat compares only S fronts. A hash map id -> (size,
//   order) finds a party's entry for leave without scanning.
// Pitfalls:
// - order is one global counter, not a per-bucket index, or fronts of different
//   buckets cannot be compared
// - leave erases the bucket entry and the info entry; drop an empty bucket, or
//   *bucket.begin() on it is UB
// - the bucket map is sorted by size: break at the first size > tableSize
// - scan: after line.erase(it) return at once; it is invalid after the erase
// Tests to write: earliest fit skips a too-big party; leave from the middle and the
//   front; rejoin goes to the back; second join of a waiting id ignored; seat on empty
//   / leave absent; exact-size tables.

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::list;
using std::map;
using std::set;
using std::unordered_map;
using std::pair;

// straightforward: one line in arrival order, scan from the front.
// T: join O(n) (duplicate check), leave O(n), seat O(n), S: O(n).
class WaitlistScan {
    // (partyId, size) in arrival order
    list<pair<int, int>> line;

public:
    void join(int partyId, int size) {
        for (const pair<int, int>& party : line) {
            if (party.first == partyId) { return; }
        }
        line.push_back({partyId, size});
    }

    void leave(int partyId) {
        for (auto it = line.begin(); it != line.end(); it++) {
            if (it->first == partyId) {
                line.erase(it);
                return;
            }
        }
    }

    int seat(int tableSize) {
        for (auto it = line.begin(); it != line.end(); it++) {
            if (it->second <= tableSize) {
                int partyId = it->first;
                line.erase(it);
                return partyId;
            }
        }
        return -1;
    }
};

// improved: bucket by party size; each bucket is ordered by arrival, so seat compares
// only the front of each bucket with size <= tableSize.
// T: join / leave O(log n), seat O(S + log n) with S = distinct sizes, S: O(n).
class Waitlist {
    // size -> (arrival order, partyId) of the waiting parties of that size
    map<int, set<pair<int, int>>> buckets;
    // partyId -> (size, arrival order)
    unordered_map<int, pair<int, int>> info;
    int nextOrder = 0;

public:
    void join(int partyId, int size) {
        if (info.count(partyId)) { return; }
        info[partyId] = {size, nextOrder};
        buckets[size].insert({nextOrder, partyId});
        nextOrder++;
    }

    void leave(int partyId) {
        auto it = info.find(partyId);
        if (it == info.end()) { return; }
        auto [size, order] = it->second;
        buckets[size].erase({order, partyId});
        if (buckets[size].empty()) { buckets.erase(size); }
        info.erase(it);
    }

    int seat(int tableSize) {
        int best = -1;
        int bestOrder = 0;
        for (const auto& [size, bucket] : buckets) {
            if (size > tableSize) { break; }
            auto [order, partyId] = *bucket.begin();
            if (best == -1 || order < bestOrder) {
                best = partyId;
                bestOrder = order;
            }
        }
        if (best != -1) { leave(best); }
        return best;
    }

    // reported variant: tables seat only parties of exactly tableSize. T: O(log n).
    // Pitfall: find, never buckets[tableSize]: that inserts an empty bucket, and seat
    // would then read *begin() of an empty set.
    int seatExact(int tableSize) {
        auto it = buckets.find(tableSize);
        if (it == buckets.end()) { return -1; }
        int partyId = it->second.begin()->second;
        leave(partyId);
        return partyId;
    }
};

// ops: {"join", {partyId, size}}, {"leave", {partyId}}, {"seat", {tableSize}};
// want = the results of the seat ops, in order
void test(const string& label, const vector<pair<string, vector<int>>>& ops,
          const vector<int>& want) {
    WaitlistScan scan;
    Waitlist fast;
    vector<int> gotScan;
    vector<int> gotFast;
    for (const auto& [op, args] : ops) {
        if (op == "join") {
            scan.join(args[0], args[1]);
            fast.join(args[0], args[1]);
        } else if (op == "leave") {
            scan.leave(args[0]);
            fast.leave(args[0]);
        } else {
            gotScan.push_back(scan.seat(args[0]));
            gotFast.push_back(fast.seat(args[0]));
        }
    }
    bool ok = gotScan == want && gotFast == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("earliest-fit",
         {{"join", {1, 6}}, {"join", {2, 2}}, {"join", {3, 3}}, {"seat", {4}},
          {"seat", {4}}, {"seat", {4}}, {"seat", {6}}},
         {2, 3, -1, 1});
    test("leave-middle",
         {{"join", {1, 2}}, {"join", {2, 2}}, {"join", {3, 2}}, {"leave", {2}},
          {"leave", {1}}, {"seat", {2}}, {"seat", {2}}},
         {3, -1});
    test("rejoin-back",
         {{"join", {1, 2}}, {"join", {2, 4}}, {"leave", {1}}, {"join", {1, 2}},
          {"join", {2, 3}}, {"seat", {4}}, {"seat", {4}}},
         {2, 1});
    test("dup-join-ignored",
         {{"join", {1, 2}}, {"join", {1, 5}}, {"seat", {3}}, {"seat", {5}}}, {1, -1});
    test("empty", {{"leave", {5}}, {"seat", {10}}}, {-1});

    Waitlist exact;
    exact.join(1, 4);
    exact.join(2, 2);
    exact.join(3, 2);
    check(exact.seatExact(2) == 2 && exact.seatExact(2) == 3 && exact.seatExact(3) == -1,
          "seat-exact");
}

// follow-up: party sizes bounded (<= 20) -> vector<set<pair<int, int>>> buckets(21)
//   indexed by size; seat walks sizes 1..min(tableSize, 20), skipping empty buckets
// follow-up: seat as many parties as fit at one big table, in line order (greedy, not
//   the best packing) -> in seat, save seatedSize = info[best].first before leave(best):
//     vector<int> seated;
//     int partyId = seat(tableSize);
//     while (partyId != -1) {
//         seated.push_back(partyId);
//         tableSize -= seatedSize;
//         partyId = seat(tableSize);
//     }
// follow-up: seat the largest fitting party (earliest of that size) instead:
//     auto it = buckets.upper_bound(tableSize);
//     if (it == buckets.begin()) { return -1; }
//     int partyId = prev(it)->second.begin()->second;
//     leave(partyId);
//     return partyId;
// follow-up: position in line / estimated wait -> parties ahead = waiting parties with a
//   smaller order: walk the scan list, O(n); or a Fenwick tree over orders (+1 on join,
//   -1 on leave), ahead = prefix sum up to order - 1, O(log n); wait ~ ahead x average
//   minutes per party (ask what turnover data exists)
