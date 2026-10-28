// LC 635: put(id, timestamp) stores a log with a fixed-width timestamp
// "Year:Month:Day:Hour:Minute:Second", e.g. "2024:01:31:09:00:00".
// retrieve(start, end, granularity) returns the ids whose timestamp lies in [start, end]
// when every timestamp is cut down to the granularity (Year, Month, Day, Hour, Minute
// or Second). Any order.
// PointOne angle: querying time records at different granularities.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::multimap;
using std::sort;

// length of the timestamp prefix that a granularity keeps
int prefixLen(const string& granularity) {
    if (granularity == "Year") { return 4; }
    if (granularity == "Month") { return 7; }
    if (granularity == "Day") { return 10; }
    if (granularity == "Hour") { return 13; }
    if (granularity == "Minute") { return 16; }
    return 19;
}

// straightforward: keep every log; retrieve compares truncated strings. Fixed width
// means string order == time order.
class LogSystem {
    vector<pair<string, int>> logs;   // (timestamp, id) in insertion order

public:
    // T: O(1) amortized.
    void put(int id, const string& timestamp) { logs.push_back({timestamp, id}); }

    // T: O(n) per query.
    vector<int> retrieve(const string& start, const string& end,
                         const string& granularity) {
        int len = prefixLen(granularity);
        string lo = start.substr(0, len), hi = end.substr(0, len);
        vector<int> result;
        for (const auto& [ts, id] : logs) {
            string cut = ts.substr(0, len);
            if (cut >= lo && cut <= hi) { result.push_back(id); }
        }
        return result;
    }
};

// improved (many logs): keep them sorted by timestamp and jump to the range, padding the
// cut bounds with the smallest / largest possible tail. T: O(log n + k) per query.
class LogSystemSorted {
    multimap<string, int> logs;   // timestamp -> id, sorted by timestamp

public:
    // T: O(log n).
    void put(int id, const string& timestamp) { logs.insert({timestamp, id}); }

    vector<int> retrieve(const string& start, const string& end,
                         const string& granularity) {
        int len = prefixLen(granularity);
        string lo = start.substr(0, len) + string("0000:00:00:00:00:00").substr(len);
        string hi = end.substr(0, len) + string("9999:99:99:99:99:99").substr(len);
        vector<int> result;
        auto first = logs.lower_bound(lo);
        auto last = logs.upper_bound(hi);
        for (auto it = first; it != last; it++) { result.push_back(it->second); }
        return result;
    }
};

// canonical form for comparing: any order is accepted
vector<int> sorted(vector<int> v) {
    sort(v.begin(), v.end());
    return v;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    LogSystem a;
    LogSystemSorted b;
    vector<pair<int, string>> puts = {{1, "2024:01:31:09:00:00"},
                                      {2, "2024:02:01:10:30:00"},
                                      {3, "2023:12:31:23:59:59"}};
    for (const auto& [id, ts] : puts) {
        a.put(id, ts);
        b.put(id, ts);
    }
    string s = "2024:01:01:00:00:00", e = "2024:01:31:23:59:59";
    check(sorted(a.retrieve(s, e, "Month")) == vector<int>{1} &&
          sorted(b.retrieve(s, e, "Month")) == vector<int>{1}, "month");
    check(sorted(a.retrieve(s, e, "Year")) == vector<int>{1, 2} &&
          sorted(b.retrieve(s, e, "Year")) == vector<int>{1, 2}, "year");
    string s2 = "2024:01:31:09:00:01", e2 = "2024:02:01:10:30:00";
    check(sorted(a.retrieve(s2, e2, "Second")) == vector<int>{2} &&
          sorted(b.retrieve(s2, e2, "Second")) == vector<int>{2}, "second");
}
