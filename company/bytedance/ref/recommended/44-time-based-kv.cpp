// ByteDance (1p3a 1188918, phone round 2) / LC 981: design a time-based key-value store.
// set(key, value, timestamp) stores value for key at that time; timestamps passed to set
// are strictly increasing. get(key, timestamp) returns the value whose timestamp is the
// largest one <= timestamp, or "" if there is none (or the key was never set).
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::pair;

class TimeMap {
    // store[key] = (timestamp, value) pairs in increasing timestamp order
    unordered_map<string, vector<pair<int, string>>> store;

public:
    // Timestamps only grow, so appending keeps each list sorted. T: O(1).
    void set(const string& key, const string& value, int timestamp) {
        store[key].push_back({timestamp, value});
    }

    // Binary search for the last entry with timestamp <= t. T: O(log k) for k entries.
    string get(const string& key, int t) {
        auto it = store.find(key);
        if (it == store.end()) {
            return "";
        }
        const vector<pair<int, string>>& entries = it->second;
        int lo = 0;
        int hi = entries.size() - 1;
        int found = -1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (entries[mid].first <= t) {
                found = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        if (found == -1) {
            return "";
        }
        return entries[found].second;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TimeMap store;
    store.set("price", "10", 5);
    store.set("price", "12", 9);
    store.set("stock", "3", 7);
    check(store.get("price", 4) == "", "before-first");
    check(store.get("price", 5) == "10", "exact");
    check(store.get("price", 8) == "10", "between");
    check(store.get("price", 100) == "12", "after-last");
    check(store.get("stock", 7) == "3", "other-key");
    check(store.get("color", 7) == "", "missing-key");
}
