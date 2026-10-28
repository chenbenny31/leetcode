// LC 636: n functions on one thread; logs "id:start:t" / "id:end:t" in time order, where
// end t means the call finishes at the end of unit t. A running call pauses while a
// nested call runs. Return the exclusive time of each function.
// PointOne angle: turning an activity log into time entries.
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;
using std::stoi;

// split s on sep with a plain character loop
vector<string> split(const string& s, char sep) {
    vector<string> parts;
    string cur;
    for (char c : s) {
        if (c == sep) {
            parts.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    parts.push_back(cur);
    return parts;
}

// Stack of running calls; each event charges the time since the last event to the call
// on top. T: O(L) for L logs, S: O(L).
vector<int> exclusiveTime(int n, const vector<string>& logs) {
    vector<int> result(n, 0);   // result[id] = exclusive time of function id
    vector<int> stk;            // ids of calls in progress, innermost on top
    int prev = 0;               // start of the slice not charged to anyone yet
    for (const auto& log : logs) {
        vector<string> parts = split(log, ':');
        int id = stoi(parts[0]);
        int t = stoi(parts[2]);
        if (parts[1] == "start") {
            // the caller ran until just before t
            if (!stk.empty()) { result[stk.back()] += t - prev; }
            stk.push_back(id);
            prev = t;
        } else {
            // end t is inclusive: the call also owns unit t
            result[stk.back()] += t - prev + 1;
            stk.pop_back();
            prev = t + 1;
        }
    }
    return result;
}

void test(const string& label, int n, const vector<string>& logs,
          const vector<int>& want) {
    cout << label << (exclusiveTime(n, logs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", 1, {"0:start:0", "0:end:2"}, {3});
    test("nested", 2, {"0:start:0", "1:start:1", "1:end:1", "0:end:2"}, {2, 1});
    test("recursive", 1, {"0:start:0", "0:start:1", "0:end:1", "0:end:2"}, {3});
}
