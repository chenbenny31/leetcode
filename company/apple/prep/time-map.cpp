// set stores a value, get return the value set at least timestamp <- cur

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
private:
    unordered_map<string, vector<pair<int, string>>> data; // data[key] = {ts, val}

public:
    // append keeps each list sorted, T: O(1)
    void set(const string& key, const string& value, int timestamp) {
        data[key].push_back({timestamp, value});
    }

    // binary-search for first entry after timestamp then step back one, T: l(logn)
    string get(const string& key, int timestamp) {
        auto it = data.find(key);
        if (it == data.end()) { return ""; }
        const auto& vals = it->second;
        int lo = 0;
        int hi = vals.size();
        while (lo < hi) { // upperBound: first elem > target
            int mid = lo + (hi - lo) / 2;
            if (vals[mid].first <= timestamp) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        return lo == 0 ? "" : vals[lo - 1].second;
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
