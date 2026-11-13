// Restaurant Waitlist (Google, interview report)
// join(partyId, size): join back of line; leave(partyId): leave from any position
// seat(tableSize): remove + return earliest-joined party w/ size <= tableSize, -1 if none
// reported: variant where a table seats only parties of exactly its size
//
// clarify:
// earliest fit or largest fit -> earliest
// join of an id already waiting -> ignore
// leave of an absent id -> no-op
// rejoin after leave -> goes to the back
// num of distinct sizes -> small, seat may walk them

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

// solution 1: brute force; baseline to state first
// core idea: one line in arrival order, scan from the front
// T: O(n) join / leave / seat, S: O(n)
class WaitlistScan {
    // line = (partyId, size) in arrival order
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

// solution 2: bucket per size; fast seat when distinct sizes are few
// core idea: bucket per size ordered by global arrival counter; seat compares fronts
// map id -> (size, order) lets leave erase w/o scanning
// T: O(log n) join / leave, O(D + log n) seat, S: O(n) — D = distinct sizes
class Waitlist {
    // buckets[size] = (arrival order, partyId) of waiting parties of that size
    map<int, set<pair<int, int>>> buckets;
    // info[partyId] = (size, arrival order)
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

    // reported variant: exact size only; T: O(log n)
    // find, never buckets[tableSize]: that would insert an empty bucket
    int seatExact(int tableSize) {
        auto it = buckets.find(tableSize);
        if (it == buckets.end()) { return -1; }
        int partyId = it->second.begin()->second;
        leave(partyId);
        return partyId;
    }
};

// ops: join {partyId, size} / leave {partyId} / seat {tableSize}; want = seat results
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

    Waitlist exact;
    exact.join(1, 4);
    exact.join(2, 2);
    exact.join(3, 2);
    check(exact.seatExact(2) == 2 && exact.seatExact(2) == 3 && exact.seatExact(3) == -1,
          "seat-exact");
}

// follow-up:
// sizes bounded (<= 20) -> vector of buckets by size; seat walks 1..min(tableSize, 20)
// fill one big table in line order -> call seat repeatedly, tableSize -= size, until -1
// seat largest fitting party -> buckets.upper_bound(tableSize), step back, take its front
// concurrent calls -> one mutex around join / leave / seat; both structures move together
// leave must erase from both structures -> else a ghost entry gets seated later
// split a big party over tables -> ask; default no
// position in line / wait estimate -> Fenwick over orders (+1 join, -1 leave), prefix sum
