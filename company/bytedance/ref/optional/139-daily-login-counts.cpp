// ByteDance (hack2hire): records (timestamp "YYYY-MM-DD HH:MM:SS", date "YYYY-MM-DD",
// username). Skip invalid records (bad format or calendar value, timestamp day != date,
// empty username); count valid logins per (username, date), sorted by user then date.

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::tuple;

struct Record {
    string timestamp;
    string date;
    string user;
};

// fmt: 'd' = any digit, every other char must match exactly
bool matches(const string& s, const string& fmt) {
    if (s.size() != fmt.size()) {
        return false;
    }
    for (int i = 0; i < (int)s.size(); i++) {
        if (fmt[i] == 'd') {
            if (s[i] < '0' || s[i] > '9') {
                return false;
            }
        } else if (s[i] != fmt[i]) {
            return false;
        }
    }
    return true;
}

// value of the len digits starting at pos (already checked to be digits)
int num(const string& s, int pos, int len) {
    int v = 0;
    for (int i = pos; i < pos + len; i++) {
        v = v * 10 + (s[i] - '0');
    }
    return v;
}

bool validDate(const string& d) {
    if (!matches(d, "dddd-dd-dd")) {
        return false;
    }
    int y = num(d, 0, 4);
    int m = num(d, 5, 2);
    int day = num(d, 8, 2);
    if (m < 1 || m > 12) {
        return false;
    }
    // days per month in a non-leap year
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
    int maxDay = (m == 2 && leap) ? 29 : days[m - 1];
    return day >= 1 && day <= maxDay;
}

bool validTimestamp(const string& ts) {
    if (!matches(ts, "dddd-dd-dd dd:dd:dd")) {
        return false;
    }
    if (!validDate(ts.substr(0, 10))) {
        return false;
    }
    return num(ts, 11, 2) < 24 && num(ts, 14, 2) < 60 && num(ts, 17, 2) < 60;
}

// "" when r is valid, else the first problem found
string invalidReason(const Record& r) {
    if (!validTimestamp(r.timestamp)) {
        return "bad timestamp";
    }
    if (!validDate(r.date)) {
        return "bad date";
    }
    if (r.timestamp.substr(0, 10) != r.date) {
        return "date mismatch";
    }
    if (r.user.empty()) {
        return "empty username";
    }
    return "";
}

// Validate each record, count in an ordered map. T: O(n log n), S: O(n).
vector<tuple<string, string, int>> dailyLogins(const vector<Record>& records) {
    // cnt[(user, date)] = valid logins, sorted by user then date
    map<pair<string, string>, int> cnt;
    for (const Record& r : records) {
        if (!invalidReason(r).empty()) {
            continue;
        }
        cnt[{r.user, r.date}]++;
    }
    vector<tuple<string, string, int>> result;
    for (const auto& [key, c] : cnt) {
        result.push_back({key.first, key.second, c});
    }
    return result;
}

// follow-up: (index, reason) for every invalid record. T: O(n), S: O(n).
vector<pair<int, string>> invalidRecords(const vector<Record>& records) {
    // (record index, why it was skipped)
    vector<pair<int, string>> bad;
    for (int i = 0; i < (int)records.size(); i++) {
        string why = invalidReason(records[i]);
        if (!why.empty()) {
            bad.push_back({i, why});
        }
    }
    return bad;
}

void test(const string& label, const vector<Record>& records,
          const vector<tuple<string, string, int>>& want) {
    cout << label << (dailyLogins(records) == want ? " PASS\n" : " FAIL\n");
}

void testInvalid(const string& label, const vector<Record>& records,
                 const vector<pair<int, string>>& want) {
    cout << label << (invalidRecords(records) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<Record> records = {
        {"2024-06-01 09:00:00", "2024-06-01", "alice"},
        {"2024-06-01 18:30:00", "2024-06-01", "alice"},
        {"2024-06-02 08:00:00", "2024-06-02", "alice"},
        {"2024-06-01 10:00:00", "2024-06-01", "bob"},
        {"2024-06-01 25:00:00", "2024-06-01", "bob"},
        {"2024-06-03 10:00:00", "2024-06-02", "carol"},
        {"2024-06-01 11:00:00", "2024-06-01", ""},
    };
    test("example", records, {{"alice", "2024-06-01", 2}, {"alice", "2024-06-02", 1},
                              {"bob", "2024-06-01", 1}});
    test("feb-29", {
        {"2023-02-29 10:00:00", "2023-02-29", "alice"},
        {"2024-02-29 10:00:00", "2024-02-29", "alice"},
    }, {{"alice", "2024-02-29", 1}});
    testInvalid("reasons", records,
                {{4, "bad timestamp"}, {5, "date mismatch"}, {6, "empty username"}});
}

// follow-up: report invalid records -> see invalidRecords() above; invalidReason()
//   returns the first failed check
// follow-up: distinct users per day -> map<string, set<string>> users[date]
// follow-up: longest login streak per user -> set<int> of day numbers (days since epoch,
//   handles month/year wrap); longest run of consecutive ints, O(n log n)
