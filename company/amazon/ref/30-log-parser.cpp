// Parse log lines "<ISO timestamp> <LEVEL> <source>: <message> [k1=v1 k2=v2 ...]" into
// records. LEVEL is DEBUG/INFO/WARN/ERROR; trailing key=value tokens are attrs, and a
// value may be quoted and contain spaces. A non-record line that starts with whitespace
// (a stack-trace line) is appended to the previous record's message; any other bad line
// is skipped and counted as a warning. Queries:
// - filter(from, to, level, source, attrs): "" or an empty map means "any"
// - countBy(field) and topK(field, k), field = "level", "source", or an attr key;
//   topK sorts by count descending, then value ascending.

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;
using std::sort;

struct Record {
    string timestamp, level, source, message;
    // attrs[key] = value
    map<string, string> attrs;
};

// split on spaces; text inside double quotes stays in one token (quotes dropped)
vector<string> tokenize(const string& s) {
    vector<string> tokens;
    string cur;
    bool quoted = false;
    for (char c : s) {
        if (c == '"') {
            quoted = !quoted;
        } else if (c == ' ' && !quoted) {
            if (!cur.empty()) { tokens.push_back(cur); }
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!cur.empty()) { tokens.push_back(cur); }
    return tokens;
}

// shape check for 2026-04-25T08:13:21Z
bool isTimestamp(const string& t) {
    return t.size() == 20 && t[4] == '-' && t[7] == '-' && t[10] == 'T' && t[13] == ':' &&
           t[16] == ':' && t[19] == 'Z';
}

// fill rec from a record line; false if the line is not a record
bool parseRecord(const string& line, Record& rec) {
    static const set<string> levels = {"DEBUG", "INFO", "WARN", "ERROR"};
    vector<string> tokens = tokenize(line);
    int n = tokens.size();
    if (n < 3 || !isTimestamp(tokens[0]) || !levels.count(tokens[1])) { return false; }
    if (tokens[2].size() < 2 || tokens[2].back() != ':') { return false; }
    rec.timestamp = tokens[0];
    rec.level = tokens[1];
    rec.source = tokens[2].substr(0, tokens[2].size() - 1);
    // attrs are the trailing tokens that contain '='
    int end = n;
    while (end > 3 && tokens[end - 1].find('=') != string::npos) {
        const string& kv = tokens[end - 1];
        int eq = kv.find('=');
        rec.attrs[kv.substr(0, eq)] = kv.substr(eq + 1);
        end--;
    }
    for (int i = 3; i < end; i++) {
        if (i > 3) { rec.message += ' '; }
        rec.message += tokens[i];
    }
    return true;
}

// T: add O(line length); filter / countBy O(records); topK O(r + v log v). S: O(input).
class LogStore {
    vector<Record> records;
    int skipped = 0;

    // the value of field in r; false if r has no such field
    bool fieldValue(const Record& r, const string& field, string& value) const {
        if (field == "level") {
            value = r.level;
        } else if (field == "source") {
            value = r.source;
        } else if (r.attrs.count(field)) {
            value = r.attrs.at(field);
        } else {
            return false;
        }
        return true;
    }

public:
    void add(const string& line) {
        Record rec;
        if (parseRecord(line, rec)) {
            records.push_back(rec);
            return;
        }
        bool indented = !line.empty() && (line[0] == ' ' || line[0] == '\t');
        if (indented && !records.empty()) {
            records.back().message += "\n" + line;
            return;
        }
        skipped++;
    }

    int size() const { return records.size(); }
    int warnings() const { return skipped; }

    vector<Record> filter(const string& from, const string& to, const string& level,
                          const string& source, const map<string, string>& attrs) const {
        vector<Record> result;
        for (const Record& r : records) {
            if (!from.empty() && r.timestamp < from) { continue; }
            if (!to.empty() && r.timestamp > to) { continue; }
            if (!level.empty() && r.level != level) { continue; }
            if (!source.empty() && r.source != source) { continue; }
            bool ok = true;
            for (const auto& [k, v] : attrs) {
                if (!r.attrs.count(k) || r.attrs.at(k) != v) { ok = false; }
            }
            if (ok) { result.push_back(r); }
        }
        return result;
    }

    map<string, int> countBy(const string& field) const {
        map<string, int> counts;
        for (const Record& r : records) {
            string value;
            if (fieldValue(r, field, value)) { counts[value]++; }
        }
        return counts;
    }

    vector<pair<string, int>> topK(const string& field, int k) const {
        map<string, int> counts = countBy(field);
        vector<pair<string, int>> items(counts.begin(), counts.end());
        auto better = [](const pair<string, int>& a, const pair<string, int>& b) {
            if (a.second != b.second) { return a.second > b.second; }
            return a.first < b.first;
        };
        sort(items.begin(), items.end(), better);
        if ((int)items.size() > k) { items.resize(k); }
        return items;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    LogStore store;
    vector<string> lines = {
        "2026-04-25T08:00:00Z INFO auth: login ok user=7 ip=1.2.3.4",
        "2026-04-25T08:00:05Z ERROR pay: charge failed user=7 reason=\"card declined\"",
        "    at Charge.run(Pay.java:10)",
        "2026-04-25T08:00:09Z WARN auth: slow login user=9",
        "garbage line",
        "2026-04-25T08:00:10Z TRACE auth: bad level"};
    for (const string& line : lines) { store.add(line); }
    check(store.size() == 3 && store.warnings() == 2, "skip");
    vector<Record> errors = store.filter("", "", "ERROR", "", {});
    check(errors.size() == 1 && errors[0].attrs["reason"] == "card declined", "quoted");
    string joined = "charge failed\n    at Charge.run(Pay.java:10)";
    check(errors[0].message == joined, "continue");
    string from = "2026-04-25T08:00:00Z", to = "2026-04-25T08:00:05Z";
    check(store.filter(from, to, "", "", {}).size() == 2, "time-range");
    check(store.filter("", "", "", "", {{"user", "7"}}).size() == 2, "attr");
    map<string, int> bySource = {{"auth", 2}, {"pay", 1}};
    check(store.countBy("source") == bySource, "count");
    vector<pair<string, int>> top = {{"ERROR", 1}, {"INFO", 1}};
    check(store.topK("level", 2) == top, "top-k");
}
