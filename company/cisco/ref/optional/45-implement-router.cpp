// 45 · Implement Router (Cisco LC tag, Medium) — LC 3508, queue + set + binary search
// Router(memoryLimit): holds at most memoryLimit packets {source, destination, timestamp}
// addPacket: false if the same packet is stored; if full, drop the oldest first
// forwardPacket: remove and return the oldest packet, {} if none
// getCount(dest, start, end): stored packets to dest with timestamp in [start, end]
//
// clarify:
// addPacket timestamps -> non-decreasing across calls (given)
// duplicate -> all three fields equal; legal again after that packet left
// range ends -> inclusive
// forward on empty -> empty vector

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::set;
using std::unordered_map;
using std::lower_bound;
using std::upper_bound;

// core idea: packets leave in arrival order, so per destination the removed ones are a
// prefix of its (sorted) timestamp list: keep the list + a prefix count, binary search
// T: addPacket / forwardPacket / getCount O(log n), S: O(packets ever added)
class Router {
    int limit;
    // stored packets in arrival order, front = oldest
    queue<vector<int>> packets;
    // stored packets, for the duplicate check
    set<vector<int>> stored;
    // destination -> timestamps of every packet ever added to it (non-decreasing)
    unordered_map<int, vector<int>> times;
    // destination -> how many of its timestamps already left (a prefix of times)
    unordered_map<int, int> removed;

    vector<int> popOldest() {
        vector<int> packet = packets.front();
        packets.pop();
        stored.erase(packet);
        removed[packet[1]]++;
        return packet;
    }

public:
    Router(int memoryLimit) : limit(memoryLimit) {}

    bool addPacket(int source, int destination, int timestamp) {
        vector<int> packet = {source, destination, timestamp};
        if (stored.count(packet) > 0) { return false; }
        if ((int)packets.size() == limit) { popOldest(); }
        packets.push(packet);
        stored.insert(packet);
        times[destination].push_back(timestamp);
        return true;
    }

    vector<int> forwardPacket() {
        if (packets.empty()) { return {}; }
        return popOldest();
    }

    int getCount(int destination, int startTime, int endTime) {
        auto it = times.find(destination);
        if (it == times.end()) { return 0; }
        const vector<int>& ts = it->second;
        // search only the part that is still stored
        auto first = ts.begin() + removed[destination];
        auto lo = lower_bound(first, ts.end(), startTime);
        auto hi = upper_bound(first, ts.end(), endTime);
        return (int)(hi - lo);
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Router router(3);
    router.addPacket(1, 4, 90);
    router.addPacket(2, 5, 90);
    check(!router.addPacket(1, 4, 90), "duplicate");
    router.addPacket(3, 5, 95);
    router.addPacket(4, 5, 105);
    check(router.forwardPacket() == vector<int>({2, 5, 90}), "evict-then-forward-oldest");
    check(router.getCount(5, 90, 110) == 2, "count-skips-removed");

    Router empty(1);
    check(empty.forwardPacket().empty(), "forward-empty");
    return 0;
}

// follow-up:
// 1. memory grows forever -> deque per destination, pop_front on removal
// 2. timestamps not sorted -> ordered multiset per destination, or a Fenwick tree on time
// 3. faster duplicate check -> hash the 3 fields into one key (unordered_set)
// 4. count per source too -> a second map keyed by source, same prefix trick
// 5. drop by priority, not age -> ordered set keyed by priority
