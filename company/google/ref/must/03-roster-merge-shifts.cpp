// Each roster entry is [name, start, end]: that person is on shift during [start, end).
// Return the merged schedule: one [start, end, names on shift] row for every interval
// between consecutive distinct times, from the earliest start to the latest end,
// including gaps with nobody on shift. Names keep roster order.

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::set;
using std::sort;
using std::unique;

struct Shift {
    string name;
    int start;
    int end;
};

struct Row {
    int start;
    int end;
    vector<string> names;

    bool operator==(const Row& other) const {
        return start == other.start && end == other.end && names == other.names;
    }
};

// straightforward: every distinct start / end time is a cut point; for each piece between
// two neighboring cuts, list the shifts that fully cover it. T: O(P^2), S: O(P).
vector<Row> mergeRoster(const vector<Shift>& roster) {
    // every distinct start / end, ascending
    vector<int> times;
    for (const Shift& shift : roster) {
        times.push_back(shift.start);
        times.push_back(shift.end);
    }
    sort(times.begin(), times.end());
    times.erase(unique(times.begin(), times.end()), times.end());

    vector<Row> result;
    for (int t = 0; t + 1 < (int)times.size(); t++) {
        int from = times[t];
        int to = times[t + 1];
        vector<string> names;
        for (const Shift& shift : roster) {
            if (shift.start <= from && to <= shift.end) { names.push_back(shift.name); }
        }
        result.push_back({from, to, names});
    }
    return result;
}

// improved: sweep the cut points once, keeping the set of people on shift; at each time
// apply the shifts ending there, then the ones starting there.
// T: O(P log P + output), S: O(P).
vector<Row> mergeRosterSweep(const vector<Shift>& roster) {
    // startsAt[t] / endsAt[t] = roster indices whose shift starts / ends at time t
    map<int, vector<int>> startsAt;
    map<int, vector<int>> endsAt;
    // every distinct start / end, ascending
    set<int> timeSet;
    int p = roster.size();
    for (int idx = 0; idx < p; idx++) {
        startsAt[roster[idx].start].push_back(idx);
        endsAt[roster[idx].end].push_back(idx);
        timeSet.insert(roster[idx].start);
        timeSet.insert(roster[idx].end);
    }
    vector<int> times(timeSet.begin(), timeSet.end());

    // roster indices on shift right now; a set keeps them in roster order
    set<int> active;
    vector<Row> result;
    for (int t = 0; t + 1 < (int)times.size(); t++) {
        int now = times[t];
        for (int idx : endsAt[now]) { active.erase(idx); }
        for (int idx : startsAt[now]) { active.insert(idx); }
        vector<string> names;
        for (int idx : active) { names.push_back(roster[idx].name); }
        result.push_back({now, times[t + 1], names});
    }
    return result;
}

void test(const string& label, const vector<Shift>& roster, const vector<Row>& want) {
    bool ok = mergeRoster(roster) == want && mergeRosterSweep(roster) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overlap", {{"amy", 1, 3}, {"bob", 2, 4}},
         {{1, 2, {"amy"}}, {2, 3, {"amy", "bob"}}, {3, 4, {"bob"}}});
    test("gap", {{"amy", 1, 2}, {"bob", 3, 4}},
         {{1, 2, {"amy"}}, {2, 3, {}}, {3, 4, {"bob"}}});
    test("nested", {{"amy", 1, 4}, {"bob", 2, 3}},
         {{1, 2, {"amy"}}, {2, 3, {"amy", "bob"}}, {3, 4, {"amy"}}});
    test("handoff", {{"amy", 1, 2}, {"bob", 2, 3}}, {{1, 2, {"amy"}}, {2, 3, {"bob"}}});
    test("empty", {}, {});
}

// follow-up: merge neighboring rows with the same names -> compare with the last row
//   before pushing and extend its end instead
// follow-up: roster order of names not needed -> keep names in a multiset / sort them
