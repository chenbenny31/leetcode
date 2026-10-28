// Hub i has capacity capacities[i] and booked intervals intervals[i] ("start:end").
// Return the hub with enough capacity (>= required) and no booking that overlaps
// taskInterval, choosing the smallest capacity, then the smallest index; -1 if none.
// [a, b] and [c, d] overlap iff a < d and c < b.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::stoi;

// "start:end" -> (start, end) with a plain character loop
pair<int, int> parseInterval(const string& s) {
    int colon = 0;
    while (s[colon] != ':') { colon++; }
    return {stoi(s.substr(0, colon)), stoi(s.substr(colon + 1))};
}

// check every hub's capacity and bookings. T: O(n + total bookings), S: O(1).
int findHub(const vector<int>& capacities, const vector<vector<string>>& intervals,
            int required, const string& taskInterval) {
    auto [a, b] = parseInterval(taskInterval);
    int best = -1;
    int n = capacities.size();
    for (int i = 0; i < n; i++) {
        if (capacities[i] < required) { continue; }
        bool free = true;
        for (const string& booked : intervals[i]) {
            auto [c, d] = parseInterval(booked);
            if (a < d && c < b) {
                free = false;
                break;
            }
        }
        if (!free) { continue; }
        if (best == -1 || capacities[i] < capacities[best]) { best = i; }
    }
    return best;
}

void test(const string& label, const vector<int>& capacities,
          const vector<vector<string>>& intervals, int required, const string& task,
          int want) {
    bool ok = findHub(capacities, intervals, required, task) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("busy-skip", {150, 100, 200, 100}, {{}, {"10:20"}, {}, {}}, 100, "15:18", 3);
    test("touch-free", {100}, {{"20:30"}}, 50, "10:20", 0);
    test("too-small", {50}, {{}}, 100, "1:2", -1);
    test("tie-index", {100, 100}, {{}, {}}, 100, "1:2", 0);
}
