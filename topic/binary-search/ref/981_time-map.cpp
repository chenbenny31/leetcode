// LC 981 Time Based Key-Value Store: set(key, value, timestamp) stores a value; get(key,
// timestamp) returns the value set at the largest timestamp_prev <= timestamp, or "" if
// none. For each key, set is called with strictly increasing timestamps.
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
    // key -> (timestamp, value) pairs, timestamps strictly increasing
    unordered_map<string, vector<pair<int, string>>> history;

public:
    // Appending keeps each list sorted. T: O(1) amortized, S: O(1) per call.
    void set(const string& key, const string& value, int timestamp) {
        history[key].push_back({timestamp, value});
    }

    // Binary search for the first entry later than timestamp, then step back one.
    // T: O(log n) for n entries of the key, S: O(1).
    string get(const string& key, int timestamp) {
        auto it = history.find(key);
        if (it == history.end()) { return ""; }
        const auto& list = it->second;
        int lo = 0;
        int hi = list.size();
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (list[mid].first <= timestamp) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        // lo == 0: every entry is later than timestamp
        if (lo == 0) { return ""; }
        return list[lo - 1].second;
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
    check(m.get("k", 3) == "v1" && m.get("k", 4) == "v2", "versions");
    check(m.get("k", 0) == "" && m.get("x", 5) == "", "miss");
}
