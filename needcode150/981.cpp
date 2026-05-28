// hash-map + binary-search, T: O(log(n)) get, O(1) set, S: O(1)

#include <string>
#include <unordered_map>
#include <vector>
#include <utility> // std::pair
#include <string_view>

class TimeMap {
private:
    using Entry = std::pair<int, std::string>;
    using Bucket = std::vector<Entry>;
    std::unordered_map<std::string, Bucket> store;

public:
    TimeMap() {
        store.reserve(128);
        store.max_load_factor(0.25f);
    }

    void set(std::string key, std::string value, int timestamp) {
        store[key].push_back({timestamp, std::move(value)});
    }

    std::string get(std::string key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) { return ""; }

        auto& vec = it->second;
        int lo = 0;
        int hi = static_cast<int>(vec.size()) - 1;
        int res = -1;

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (vec[mid].first <= timestamp) { res = mid; lo = mid + 1; }
            else { hi = mid - 1; }
        }
        return res == -1 ? "" : vec[res].second;
    }
};

// self-impl binarys search: find largest timestamp <= given, equivalent to upper_bound - 1
// std::move(value): avoid string copy
// type alias Entry and Bucket: replace deeply nested type in map declaration
// strictly incre timestamps: push_back already maintains sorted order
// cache two levels of indirection: hash-map to vector, binary search within vector
// non-incre timestamps: insertion-sort
