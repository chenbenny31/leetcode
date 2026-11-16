// Train Schedule Route Feasibility (Google, interview report); same for flights
// (1) trips (from, to, depart, arrive); start at src at earliestStart; can we reach dst?
// (2) earliest arrival time at dst, -1 if unreachable
// (3) least total waiting between trips on a route src -> dst, -1 if unreachable
// reported: x2 (2027 intern: flights, (1) then (3); L4 onsite: trains, (1))
//
// clarify:
// arrive at 4, depart at 4 -> allowed
// depart < arrive always -> sorting by depart is a valid order
// trip leaving src before earliestStart -> not usable
// src == dst -> needs a trip arriving back at src
// bool or earliest arrival -> follow-up (2)
// (3) wait = next depart - prev arrive; nothing before the 1st trip

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <functional>
#include <utility>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::priority_queue;
using std::greater;
using std::pair;
using std::sort;

struct Trip {
    string from;
    string to;
    int depart;
    int arrive;
};

// (1) reachable
// core idea: sort by depart, scan once, keep earliest time we can be at each station
// a trip feeding a departure at d departed before d -> already processed
// T: O(T log T), S: O(T)
bool canTravel(vector<Trip> trips, const string& src, const string& dst,
               int earliestStart = 0) {
    sort(trips.begin(), trips.end(), [](const Trip& a, const Trip& b) {
        return a.depart < b.depart;
    });
    // earliest[station] = earliest time we can stand at station; absent = unreachable
    unordered_map<string, int> earliest;
    earliest[src] = earliestStart;
    bool reached = false;
    for (const Trip& trip : trips) {
        auto it = earliest.find(trip.from);
        if (it == earliest.end() || it->second > trip.depart) { continue; }
        if (trip.to == dst) { reached = true; }
        if (!earliest.count(trip.to) || trip.arrive < earliest[trip.to]) {
            earliest[trip.to] = trip.arrive;
        }
    }
    return reached;
}

// (2) earliest arrival
// core idea: same scan; answer = min arrive over usable trips into dst
// a later departure can arrive earlier -> don't stop at the first hit
// T: O(T log T), S: O(T)
int earliestArrival(vector<Trip> trips, const string& src, const string& dst,
                    int earliestStart = 0) {
    sort(trips.begin(), trips.end(), [](const Trip& a, const Trip& b) {
        return a.depart < b.depart;
    });
    // earliest[station] = earliest time we can stand at station; absent = unreachable
    unordered_map<string, int> earliest;
    earliest[src] = earliestStart;
    int best = -1;
    for (const Trip& trip : trips) {
        auto it = earliest.find(trip.from);
        if (it == earliest.end() || it->second > trip.depart) { continue; }
        if (trip.to == dst && (best == -1 || trip.arrive < best)) { best = trip.arrive; }
        if (!earliest.count(trip.to) || trip.arrive < earliest[trip.to]) {
            earliest[trip.to] = trip.arrive;
        }
    }
    return best;
}

// (3) least total waiting
// core idea: Dijkstra over trips; trip f -> g if f.to == g.from and g.depart >= f.arrive
// edge cost = g.depart - f.arrive; every trip leaving src starts at cost 0
// T: O(T^2 log T), S: O(T) — a station w/ many trips gives up to T^2 edges
int minTotalWait(const vector<Trip>& trips, const string& src, const string& dst) {
    int n = trips.size();
    // leaving[station] = idx of the trips that depart from station
    unordered_map<string, vector<int>> leaving;
    for (int i = 0; i < n; i++) { leaving[trips[i].from].push_back(i); }
    // dist[i] = least total wait so far to be riding trip i
    vector<int> dist(n, INT_MAX);
    // heap = (total wait, trip), smallest wait on top
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    for (int i : leaving[src]) {
        dist[i] = 0;
        heap.push({0, i});
    }
    while (!heap.empty()) {
        auto [wait, cur] = heap.top();
        heap.pop();
        if (wait > dist[cur]) { continue; }
        if (trips[cur].to == dst) { return wait; }
        for (int nxt : leaving[trips[cur].to]) {
            int gap = trips[nxt].depart - trips[cur].arrive;
            if (gap < 0 || wait + gap >= dist[nxt]) { continue; }
            dist[nxt] = wait + gap;
            heap.push({dist[nxt], nxt});
        }
    }
    return -1;
}

void test(const string& label, const vector<Trip>& trips, const string& src,
          const string& dst, int earliestStart, bool want) {
    bool ok = canTravel(trips, src, dst, earliestStart) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testArrival(const string& label, const vector<Trip>& trips, const string& src,
                 const string& dst, int want) {
    bool ok = earliestArrival(trips, src, dst) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testWait(const string& label, const vector<Trip>& trips, const string& src,
              const string& dst, int want) {
    cout << label << (minTotalWait(trips, src, dst) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("transfer", {{"NYC", "DC", 3, 5}, {"BOS", "NYC", 1, 3}}, "BOS", "DC", 0, true);
    test("keep-earliest",
         {{"BOS", "NYC", 1, 3}, {"BOS", "NYC", 2, 9}, {"NYC", "DC", 4, 6}},
         "BOS", "DC", 0, true);
    test("late-start", {{"BOS", "NYC", 1, 3}}, "BOS", "NYC", 2, false);
    test("round-trip", {{"BOS", "NYC", 1, 2}, {"NYC", "BOS", 3, 4}}, "BOS", "BOS", 0,
         true);
    testArrival("arrive-later-faster", {{"BOS", "NYC", 1, 10}, {"BOS", "NYC", 2, 5}},
                "BOS", "NYC", 5);
    testArrival("arrive-transfer-beats-direct",
                {{"BOS", "DC", 1, 9}, {"BOS", "NYC", 1, 2}, {"NYC", "DC", 3, 4}},
                "BOS", "DC", 4);
    testArrival("arrive-unreachable", {{"NYC", "DC", 1, 2}}, "BOS", "DC", -1);
    testArrival("arrive-round-trip", {{"BOS", "NYC", 1, 2}, {"NYC", "BOS", 3, 4}},
                "BOS", "BOS", 4);
    testWait("wait-direct", {{"BOS", "NYC", 1, 2}}, "BOS", "NYC", 0);
    testWait("wait-later-first-trip",
             {{"BOS", "NYC", 1, 3}, {"BOS", "NYC", 5, 8}, {"NYC", "DC", 10, 12}},
             "BOS", "DC", 2);
    testWait("wait-more-stops-less-wait",
             {{"BOS", "NYC", 1, 2}, {"NYC", "DC", 9, 10}, {"BOS", "PHL", 1, 3},
              {"PHL", "NYC", 3, 5}, {"NYC", "DC", 5, 6}},
             "BOS", "DC", 0);
    testWait("wait-missed-connection", {{"BOS", "NYC", 1, 5}, {"NYC", "DC", 4, 6}},
             "BOS", "DC", -1);
}

// follow-up:
// min layover L -> store arrive + L (time ready to board) in earliest
// (3) why not the scan of (1) -> it keeps the earliest arrival; least wait may come later
// (3) fewer edges -> node per (station, time): wait edge = the gap, ride edge = 0
// (3) src == dst -> 0 w/o flying; ask if a round trip is meant
// fewest transfers -> BFS over trips, same edges w/ cost 1
// return the trips taken -> via[trip.to] = trip idx when earliest improves; walk back
// repeating schedule (every P min) -> Dijkstra on time; wait = ((depart - t) % P + P) % P
