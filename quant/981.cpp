// hash-map + binary-search

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

class TimeMap {
private:
    using Entry = std::pair<int, std::string>;
    using Bucket = std::vector<Entry>;
    std::unordered_map<std::string, Bucket> map;

public:
    TimeMap() {
        map.reserve(128);
        map.max_load_factor(0.25f);
    }

    void set(std::string key, std::string value, int timestamp) {
        map[key].push_back({timestamp, std::move(value)});
    }

    std::string get(std::string key, int timestamp) {
        auto it = map.find(key);
        if (it == map.end()) { return ""; }

        auto& vec = it->second;
        int lo = 0;
        int hi = static_cast<int>(vec.size()) - 1;

        while (lo <= hi) { // upper_bound
            int mid = lo + (hi - lo) / 2;
            if (vec[mid].first <= timestamp) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        return lo == 0 ? "" : vec[lo - 1].second;
    }
};
