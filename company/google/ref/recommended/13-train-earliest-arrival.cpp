// Train Schedule Route Feasibility (Google, interview report); same for flights.
// Each trip is (from, to, departure, arrival) with departure < arrival. Starting at src
// at time earliestStart (default 0), can we reach dst taking one or more trips? A trip
// can be boarded if we are at its station by its departure (arrive at 4, leave at 4 is
// fine). src == dst counts only if some trip arrives back at src.
//
// Clarify:
// - integer times; arriving at 4 and leaving at 4 is allowed (<=)
// - departure < arrival for every trip -> given; the one-pass argument needs it
// - stations are strings -> hash map keyed by name
// - a trip leaving src before earliestStart cannot be taken
// - src == dst -> true only through a trip back into src (stated)
// - up to ~1e5 trips -> sort + one scan, O(T log T)
// - return a bool; the earliest arrival time is the follow-up function
// Idea: sort by departure and keep earliest[station] = earliest time we can stand there.
//   A trip is usable iff earliest[from] <= depart, and then it may lower earliest[to].
//   A trip that could feed a departure at d arrives by d, so it departed before d and
//   was scanned already: one pass suffices.
// Pitfalls:
// - <= at the transfer, not <
// - keep the min arrival per station; overwriting with a later one loses connections
// - earliest[to] on a missing station inserts 0, which beats every arrival: test
//   count (or find) first, as the code does
// - seed earliest[src] = earliestStart, not 0; src == dst must not return true early
// Tests to write: a transfer listed out of order; a missed connection; keep the earlier
//   of two arrivals; start after the only departure; round trip back to src.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;

struct Trip {
    string from;
    string to;
    int depart;
    int arrive;
};

// sort trips by departure and scan once, keeping the earliest time we can be at each
// station. One pass is enough: a trip that can feed a departure at time d arrives by d,
// so it departed before d and was processed first. T: O(T log T), S: O(T).
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

// follow-up: earliest arrival time at dst, -1 if unreachable. Same scan; the answer is
// the min arrival over usable trips into dst. Pitfall: do not stop at the first usable
// trip into dst: trips go by departure, and a later departure can arrive earlier.
// T: O(T log T), S: O(T).
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

int main() {
    test("transfer", {{"NYC", "DC", 3, 5}, {"BOS", "NYC", 1, 3}}, "BOS", "DC", 0, true);
    test("missed", {{"BOS", "NYC", 1, 5}, {"NYC", "DC", 4, 6}}, "BOS", "DC", 0, false);
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
}

// follow-up: earliest arrival time at dst -> earliestArrival above
// follow-up: minimum layover L between trips -> store the time we are ready to board
//   (arrival + L) instead of the arrival; src is ready at earliestStart:
//   if (!earliest.count(trip.to) || trip.arrive + L < earliest[trip.to]) {
//       earliest[trip.to] = trip.arrive + L;
//   }
// follow-up: return the trips taken -> loop by index i over the sorted trips; when
//   earliest[trip.to] improves, set via[trip.to] = i (unordered_map<string, int>); then
//   for (string at = dst; at != src; at = trips[via[at]].from) {
//       route.push_back(via[at]);
//   }
//   and reverse route (assumes dst != src and dst reached)
// follow-up: repeating schedules (every trip also runs every P minutes) -> Dijkstra on
//   time over stations: pop (t, station), skip stale entries; for each trip out of the
//   station board its next run at or after t and relax trip.to with:
//   int wait = ((trip.depart - t) % P + P) % P;
//   int arrive = t + wait + (trip.arrive - trip.depart);
