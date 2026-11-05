// ByteDance (1p3a 1188918, phone round 2) / LC 981: set(key, value, timestamp) with
// strictly increasing timestamps; get(key, timestamp) returns the value with the largest
// timestamp <= timestamp, or "" if there is none (or the key was never set).
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

// Per key, a vector of (timestamp, value) kept sorted by appending; binary search on get.
// T: O(1) set, O(log k) get for k entries of the key; S: O(total sets).
class TimeMap {
    // store[key] = (timestamp, value) pairs in increasing timestamp order
    unordered_map<string, vector<pair<int, string>>> store;

public:
    // timestamps only grow, so appending keeps each list sorted
    void set(const string& key, const string& value, int timestamp) {
        store[key].push_back({timestamp, value});
    }

    // binary search for the last entry with timestamp <= t
    string get(const string& key, int t) {
        if (!store.count(key)) {
            return "";
        }
        const vector<pair<int, string>>& entries = store[key];
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
    check(store.get("color", 7) == "", "missing-key");
}

// follow-up: timestamps not increasing -> map<int, string> per key; upper_bound(t), then
//   --it unless it == begin(); O(log k) set and get
// follow-up: shorter get -> upper_bound on the vector with a (t, entry) comparator,
//   then step back one
// follow-up: thread-safe -> shared_mutex: get takes shared_lock, set unique_lock
// follow-up: bounded memory -> keep only the last N versions per key (deque, pop_front)
