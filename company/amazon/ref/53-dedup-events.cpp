// events[i] = (eventId, timestamp); ids and timestamps may be any integers. Clarify which
// dedup is wanted, then provide both:
// 1. latest per id: one (id, max timestamp) per id, sorted by id;
// 2. history per id: distinct timestamps per id, most recent first (a stack view).

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;
using std::greater;

// one pass keeping the max per id; an ordered map gives id order. T: O(n log u).
vector<pair<int, int>> latestPerId(const vector<pair<int, int>>& events) {
    // latest[id] = max timestamp seen for id
    map<int, int> latest;
    for (const auto& [id, t] : events) {
        if (!latest.count(id) || t > latest[id]) { latest[id] = t; }
    }
    return vector<pair<int, int>>(latest.begin(), latest.end());
}

// a set per id drops exact duplicates and keeps timestamps sorted descending.
// T: O(n log n), S: O(n).
map<int, vector<int>> historyPerId(const vector<pair<int, int>>& events) {
    // seen[id] = distinct timestamps of id, newest first
    map<int, set<int, greater<int>>> seen;
    for (const auto& [id, t] : events) { seen[id].insert(t); }
    map<int, vector<int>> result;
    for (const auto& [id, ts] : seen) { result[id] = vector<int>(ts.begin(), ts.end()); }
    return result;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<pair<int, int>> events = {{2, 10}, {1, 5}, {2, 30}, {1, 5}, {-3, -1}, {0, 0}};
    vector<pair<int, int>> latest = {{-3, -1}, {0, 0}, {1, 5}, {2, 30}};
    check(latestPerId(events) == latest, "latest");
    map<int, vector<int>> history = {{-3, {-1}}, {0, {0}}, {1, {5}}, {2, {30, 10}}};
    check(historyPerId(events) == history, "history");
    check(latestPerId({}).empty(), "empty");
}
