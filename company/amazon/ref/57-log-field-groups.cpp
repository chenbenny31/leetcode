// Each log line looks like "<timestamp>|<key>=<value>;<key>=<value>;...". Lower-case the
// keys, trim spaces around values, and skip a line that has no '|', an empty timestamp, a
// pair without '=', or a missing or empty required field. Return, for every required
// field, the (timestamp, value) pairs sorted by timestamp.

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <cctype>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::tolower;
using std::stable_sort;

string trim(const string& s) {
    int lo = 0, hi = s.size();
    while (lo < hi && s[lo] == ' ') { lo++; }
    while (hi > lo && s[hi - 1] == ' ') { hi--; }
    return s.substr(lo, hi - lo);
}

// parse one line into timestamp + fields; false if malformed
bool parseLine(const string& line, string& ts, map<string, string>& fields) {
    int bar = line.find('|');
    if (bar == (int)string::npos) { return false; }
    ts = trim(line.substr(0, bar));
    if (ts.empty()) { return false; }
    // walk the items after '|', splitting on ';'
    string item;
    string rest = line.substr(bar + 1) + ";";
    for (char c : rest) {
        if (c != ';') {
            item += c;
            continue;
        }
        string kv = trim(item);
        item.clear();
        if (kv.empty()) { continue; }
        int eq = kv.find('=');
        if (eq == (int)string::npos) { return false; }
        string key = trim(kv.substr(0, eq));
        for (char& k : key) { k = tolower(k); }
        fields[key] = trim(kv.substr(eq + 1));
    }
    return true;
}

// one pass over lines, then sort each field's list. T: O(total length + r log r).
map<string, vector<pair<string, string>>> groupFields(const vector<string>& lines,
                                                      const vector<string>& required) {
    // result[field] = (timestamp, value) pairs
    map<string, vector<pair<string, string>>> result;
    for (const string& f : required) { result[f]; }
    for (const string& line : lines) {
        string ts;
        map<string, string> fields;
        if (!parseLine(line, ts, fields)) { continue; }
        bool complete = true;
        for (const string& f : required) {
            if (!fields.count(f) || fields[f].empty()) { complete = false; }
        }
        if (!complete) { continue; }
        for (const string& f : required) { result[f].push_back({ts, fields[f]}); }
    }
    for (auto& [f, list] : result) { stable_sort(list.begin(), list.end()); }
    return result;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<string> lines = {"t2|User = Bo ; Act=out", "t1|user=Al;act=in",
                            "no bar here",           "t3|user= ;act=in",
                            "t4|act=in",             "t5|user=Cy;oops"};
    auto got = groupFields(lines, {"user", "act"});
    vector<pair<string, string>> users = {{"t1", "Al"}, {"t2", "Bo"}};
    vector<pair<string, string>> acts = {{"t1", "in"}, {"t2", "out"}};
    check(got["user"] == users, "user");
    check(got["act"] == acts, "trim-lower");
    check(groupFields({"x|a=1"}, {"b"})["b"].empty(), "missing");
}
