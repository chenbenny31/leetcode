// LC 981: set(key, value, timestamp) stores a value; get(key, timestamp) returns the
// value set at the largest timestamp_prev <= timestamp, or "" if none.
// Each key's set timestamps are strictly increasing.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;

class TimeMap {
    // history[key] = (timestamp, value) pairs, timestamps strictly increasing
    unordered_map<string, vector<pair<int, string>>> history;

public:
    // Append keeps each list sorted. T: O(1) amortized.
    void set(const string& key, const string& value, int timestamp) {
        history[key].push_back({timestamp, value});
    }

    // Binary search for the first entry after timestamp, step back one. T: O(log n).
    string get(const string& key, int timestamp) {
        auto it = history.find(key);
        if (it == history.end()) { return ""; }
        const auto& h = it->second;
        int lo = 0, hi = h.size();   // first index with time > timestamp is in [lo, hi]
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (h[mid].first <= timestamp) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        // lo == 0: every entry is later than timestamp
        return lo == 0 ? "" : h[lo - 1].second;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TimeMap m;
    m.set("k", "v1", 1);
    check(m.get("k", 1) == "v1", "exact");
    check(m.get("k", 3) == "v1", "later");
    m.set("k", "v2", 4);
    check(m.get("k", 4) == "v2" && m.get("k", 3) == "v1", "versions");
    check(m.get("k", 0) == "" && m.get("x", 5) == "", "miss");
}
