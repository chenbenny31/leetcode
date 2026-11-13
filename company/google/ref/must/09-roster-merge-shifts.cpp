// Roster merge shifts — cut points / sweep
// roster entry [name, start, end] = person on shift during [start, end)
// return one [start, end, names] row per interval between consecutive distinct times
// from earliest start to latest end, gaps included; names keep roster order
//
// clarify:
// handoff at time t -> end exclusive, leaver not in the next row
// gap w/ nobody on shift -> row w/ empty names
// unsorted or empty roster -> sort the times; empty -> no rows
// zero-length shift -> adds a cut point, no name
// neighboring rows w/ same names -> stay apart (merge = follow-up)

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

// solution 1: cut points + scan; shortest to write
// core idea: every distinct start / end = cut point; per piece list shifts covering it
// T: O(P^2), S: O(P)
vector<Row> mergeRoster(const vector<Shift>& roster) {
    // times = every distinct start / end, ascending
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

// solution 2: sweep; use it when P is large
// core idea: walk cut points once, keep set of roster idx on shift
// at each time apply shifts starting there, then the ones ending there
// T: O(P log P + output), S: O(P)
vector<Row> mergeRosterSweep(const vector<Shift>& roster) {
    // startsAt[t] / endsAt[t] = roster idx whose shift starts / ends at time t
    map<int, vector<int>> startsAt;
    map<int, vector<int>> endsAt;
    // timeSet = every distinct start / end, ascending
    set<int> timeSet;
    int p = roster.size();
    for (int idx = 0; idx < p; idx++) {
        startsAt[roster[idx].start].push_back(idx);
        endsAt[roster[idx].end].push_back(idx);
        timeSet.insert(roster[idx].start);
        timeSet.insert(roster[idx].end);
    }
    vector<int> times(timeSet.begin(), timeSet.end());

    // active = roster idx on shift right now; set keeps roster order
    set<int> active;
    vector<Row> result;
    for (int t = 0; t + 1 < (int)times.size(); t++) {
        int now = times[t];
        // starts first: zero-length shift [now, now) is inserted, then erased
        for (int idx : startsAt[now]) { active.insert(idx); }
        for (int idx : endsAt[now]) { active.erase(idx); }
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
    test("handoff", {{"amy", 1, 2}, {"bob", 2, 3}}, {{1, 2, {"amy"}}, {2, 3, {"bob"}}});
    test("zero-length", {{"amy", 1, 5}, {"bob", 3, 3}},
         {{1, 3, {"amy"}}, {3, 5, {"amy"}}});
}

// follow-up:
// merge neighboring rows w/ same names -> extend result.back().end instead of pushing
// skip gap rows -> continue when names is empty, before the push
// many "who is on shift at t" queries -> build rows once, binary search row starts
// roster order of names not needed -> keep names in a multiset / sort them
