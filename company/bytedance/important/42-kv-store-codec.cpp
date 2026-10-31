// ByteDance (FastPrep, onsite): a two-way codec for a string key-value store. Each
// operation is independent and returns one row (vector<string>):
//   serialize:   payload = flat [key0, value0, key1, value1, ...] with distinct keys;
//                returns a row with the single encoded string.
//   deserialize: payload = one encoded string; if valid return the flat key/value list
//                sorted by key (ASCII order), otherwise ["ERROR"].
// Keys / values are printable ASCII, may be empty, and may contain ':' or ';'.
// Wire format: "N;" followed by exactly N records "keyLength:key" + "valueLength:value"
// (no spaces, nothing between records). N in [0, 20], every length in [0, 40]. A
// number is "0" or a decimal without leading zeros (no sign or spaces). Serialize sorts
// records by key; deserialize accepts any record order but rejects: duplicate keys,
// missing separators, truncated fields, numbers out of range, trailing characters.
// The empty store is "0;". Splitting on separators is wrong: they can appear in data.
// (The report only said "serialize / deserialize a KV store"; the format is FastPrep's.)

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cctype>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::to_string;
using std::isdigit;

// Length-prefix every string, so the decoder never has to look for a delimiter inside
// data. T: O(L log p) for L total characters and p pairs.
string serialize(const vector<string>& flat) {
    // sorted[key] = value; map orders keys by ASCII
    map<string, string> sorted;
    for (int i = 0; i + 1 < (int)flat.size(); i += 2) {
        sorted[flat[i]] = flat[i + 1];
    }
    string out = to_string(sorted.size()) + ";";
    for (auto& [key, value] : sorted) {
        out += to_string(key.size()) + ":" + key;
        out += to_string(value.size()) + ":" + value;
    }
    return out;
}

// Read a decimal at pos that ends right before the separator sep. False on a missing
// number, leading zero, value above limit, or missing separator; pos ends after sep.
bool readNumber(const string& s, int& pos, int limit, char sep, int& value) {
    int start = pos;
    value = 0;
    while (pos < (int)s.size() && isdigit(s[pos])) {
        // more than 3 digits is above every limit here; stop before overflowing
        if (pos - start >= 3) {
            return false;
        }
        value = value * 10 + (s[pos] - '0');
        pos++;
    }

    int digits = pos - start;
    if (digits == 0) {
        return false;
    }
    if (digits > 1 && s[start] == '0') {
        return false;
    }
    if (value > limit) {
        return false;
    }
    if (pos >= (int)s.size() || s[pos] != sep) {
        return false;
    }
    pos++;
    return true;
}

// Read "length:" then exactly length characters.
bool readField(const string& s, int& pos, string& field) {
    int length = 0;
    if (!readNumber(s, pos, 40, ':', length)) {
        return false;
    }
    if (pos + length > (int)s.size()) {
        return false;
    }
    field = s.substr(pos, length);
    pos += length;
    return true;
}

// A single left-to-right scan with a cursor; every check fails fast. T: O(L log N).
vector<string> deserialize(const string& s) {
    const vector<string> error = {"ERROR"};
    int pos = 0;
    int n = 0;
    if (!readNumber(s, pos, 20, ';', n)) {
        return error;
    }

    // store[key] = value, kept sorted by key
    map<string, string> store;
    for (int i = 0; i < n; i++) {
        string key;
        string value;
        if (!readField(s, pos, key) || !readField(s, pos, value)) {
            return error;
        }
        if (store.count(key)) {
            return error;
        }
        store[key] = value;
    }

    // nothing may follow the last record
    if (pos != (int)s.size()) {
        return error;
    }

    vector<string> flat;
    for (auto& [key, value] : store) {
        flat.push_back(key);
        flat.push_back(value);
    }
    return flat;
}

vector<vector<string>> kvCodec(const vector<string>& operations,
                               const vector<vector<string>>& payloads) {
    vector<vector<string>> result;
    for (int i = 0; i < (int)operations.size(); i++) {
        if (operations[i] == "serialize") {
            result.push_back({serialize(payloads[i])});
        } else {
            result.push_back(deserialize(payloads[i][0]));
        }
    }
    return result;
}

void test(const string& label, const vector<string>& operations,
          const vector<vector<string>>& payloads, const vector<vector<string>>& want) {
    cout << label << (kvCodec(operations, payloads) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("round-trip", {"serialize", "deserialize"},
         {{"x;1", "a:b", "", "e"}, {"2;0:1:e3:x;13:a:b"}},
         {{"2;0:1:e3:x;13:a:b"}, {"", "e", "x;1", "a:b"}});
    test("empty-store", {"serialize", "deserialize"}, {{}, {"0;"}}, {{"0;"}, {}});
    test("any-order", {"deserialize"}, {{"2;1:b1:21:a1:1"}}, {{"a", "1", "b", "2"}});
    test("bad-count", {"deserialize", "deserialize", "deserialize", "deserialize"},
         {{""}, {"21;"}, {"-1;"}, {"1;"}},
         {{"ERROR"}, {"ERROR"}, {"ERROR"}, {"ERROR"}});
    test("bad-record", {"deserialize", "deserialize", "deserialize", "deserialize"},
         {{"1;1:a"}, {"1;01:a1:x"}, {"1;1a1:x"}, {"1;41:a1:x"}},
         {{"ERROR"}, {"ERROR"}, {"ERROR"}, {"ERROR"}});
    test("trailing", {"deserialize"}, {{"1;1:a1:x;"}}, {{"ERROR"}});
}
