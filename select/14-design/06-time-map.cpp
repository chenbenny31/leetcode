// LC 981 Time Based Key-Value Store (Medium) — per-key history + binary search
// set(key, value, timestamp) with strictly increasing timestamps; get(key, timestamp)
// returns the value set at the largest timestamp <= timestamp, or "" if none.
// Clarify: timestamps increasing per key or globally; get before the first set;
//   missing key; the same timestamp set twice.

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

// Core idea: appending keeps each key's history sorted by time, so get binary searches
// for the last entry at or before timestamp.
// T: O(1) set, O(log k) get for k versions of the key; S: O(total sets).
class TimeMap {
    // store[key] = (timestamp, value) pairs in increasing timestamp order
    unordered_map<string, vector<pair<int, string>>> store;

public:
    void set(const string& key, const string& value, int timestamp) {
        store[key].push_back({timestamp, value});
    }

    string get(const string& key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) { return ""; }
        const vector<pair<int, string>>& entries = it->second;
        // last index with entry time <= timestamp, -1 if none
        int found = -1;
        int lo = 0;
        int hi = entries.size() - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (entries[mid].first <= timestamp) {
                found = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        return found == -1 ? "" : entries[found].second;
    }
};

void test(const string& label, const string& got, const string& want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TimeMap store;
    store.set("price", "10", 5);
    store.set("price", "12", 9);
    test("before-first", store.get("price", 4), "");
    test("between", store.get("price", 8), "10");
    test("after-last", store.get("price", 100), "12");
    test("missing-key", store.get("stock", 7), "");
    return 0;
}

// follow-up (ideas only):
// - timestamps out of order -> ordered map timestamp -> value per key; upper_bound of
//   the query, step back one ("" if it is the first); O(log k) set and get
// - thread-safe -> reader / writer lock: get takes it shared, set exclusive
