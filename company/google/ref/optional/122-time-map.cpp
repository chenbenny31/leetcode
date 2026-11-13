// 122 · Time Based Key-Value Store — LC 981
// Mother: select/14-design/06-time-map.cpp
// set(key, value, timestamp) stores a version
// get(key, timestamp) = value w/ largest timestamp <= given one, "" if none
// reported: interval membership (46)
//
// clarify:
// set timestamps -> strictly increasing per key
// unknown key -> ""
// many keys -> hash map key -> its versions

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

// core idea: per key a vector of (timestamp, value); sorted since sets come in time order
// get = binary search last entry w/ time <= timestamp
// T: set O(1), get O(log k), S: O(total sets) — k = versions of the key
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
    test("exact", store.get("price", 5), "10");
    test("between", store.get("price", 8), "10");
    test("missing-key", store.get("stock", 7), "");
}

// follow-up:
// timestamps out of order -> map<int, string> per key; upper_bound(t), then step back
// thread-safe -> shared_mutex: get takes shared lock, set unique lock
