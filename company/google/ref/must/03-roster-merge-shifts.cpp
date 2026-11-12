// Each roster entry is [name, start, end]: that person is on shift during [start, end).
// Return the merged schedule: one [start, end, names on shift] row for every interval
// between consecutive distinct times, from the earliest start to the latest end,
// including gaps with nobody on shift. Names keep roster order.
//
// Clarify:
// - [start, end) -> at a handoff time t (amy ends, bob starts) the row from t has only
//   bob
// - range of rows -> earliest start to latest end; nothing outside it; a gap row inside
//   it has no names
// - roster unsorted? -> yes; empty roster -> no rows
// - one person in two overlapping shifts? -> assume not (they would be listed twice)
// - start == end possible? -> a zero-length shift [t, t) covers nothing; it only adds a
//   cut at t
// - P (shifts) up to ~1e3 -> O(P^2) cut-point scan is fine; larger -> sweep
// - neighboring rows with the same names -> kept apart, one row per piece (follow-up)
// Idea: every start / end is a cut point; between two neighboring cuts the people on
//   shift do not change. Brute force checks every shift for each piece; the sweep keeps
//   the active set and updates it only at the cuts.
// Pitfalls:
// - a shift covers the piece [from, to) iff start <= from && to <= end
// - dedupe the times (sort + unique), or equal times give zero-length rows
// - the last time opens no row: loop while t + 1 < times.size()
// - active holds roster indices in a set<int>, not names: roster order for free, and two
//   people with the same name stay apart
// - sweep: apply the starts at t before the ends at t, or a zero-length shift stays on
// Tests to write: overlap; gap -> empty row; nested; handoff at one time; zero-length
//   shift; empty.

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
// apply the shifts starting there, then the ones ending there.
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
        // starts first: a zero-length shift [now, now) is inserted, then erased again
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
    test("nested", {{"amy", 1, 4}, {"bob", 2, 3}},
         {{1, 2, {"amy"}}, {2, 3, {"amy", "bob"}}, {3, 4, {"amy"}}});
    test("handoff", {{"amy", 1, 2}, {"bob", 2, 3}}, {{1, 2, {"amy"}}, {2, 3, {"bob"}}});
    test("zero-length", {{"amy", 1, 5}, {"bob", 3, 3}},
         {{1, 3, {"amy"}}, {3, 5, {"amy"}}});
    test("empty", {}, {});
}

// follow-up: merge neighboring rows with the same names (zero-length shifts, one person
//   back to back) -> in the sweep, instead of the push:
//     if (!result.empty() && result.back().names == names) {
//         result.back().end = times[t + 1];
//     } else {
//         result.push_back({now, times[t + 1], names});
//     }
// follow-up: skip the gap rows -> if (names.empty()) { continue; } before the push
// follow-up: many "who is on shift at time t" queries -> build the rows once, keep
//   rowStarts[i] = rows[i].start, and binary search per query, O(log P):
//     int i = upper_bound(rowStarts.begin(), rowStarts.end(), t) - rowStarts.begin() - 1;
//     if (i < 0 || t >= rows[i].end) { return {}; }
//     return rows[i].names;
// follow-up: roster order of names not needed -> keep names in a multiset / sort them
