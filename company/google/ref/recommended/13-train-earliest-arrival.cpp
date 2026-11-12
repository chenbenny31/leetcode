// Train Schedule Route Feasibility (Google, interview report); same for flights.
// Each trip is (from, to, departure, arrival) with departure < arrival. Starting at src
// at time earliestStart (default 0), can we reach dst taking one or more trips? A trip
// can be boarded if we are at its station by its departure (arrive at 4, leave at 4 is
// fine). src == dst counts only if some trip arrives back at src.

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

void test(const string& label, const vector<Trip>& trips, const string& src,
          const string& dst, int earliestStart, bool want) {
    bool ok = canTravel(trips, src, dst, earliestStart) == want;
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
}

// follow-up: earliest arrival time at dst -> return earliest[dst] (or -1); same scan
// follow-up: minimum layover L -> board only if earliest[from] + L <= departure
//   (no layover needed at src)
// follow-up: multi-day / repeating schedules -> Dijkstra on time: pop the station with
//   the smallest arrival, relax each later trip out of it to its arrival time
