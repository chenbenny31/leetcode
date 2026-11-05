// ByteDance (FastPrep, onsite): two-way codec for a string KV store. serialize(flat
// [k0, v0, k1, v1, ...]) -> "N;" + per pair (sorted by key) "len:key" + "len:value".
// deserialize(s) -> flat list sorted by key, or ["ERROR"] if malformed: N in [0, 20],
// lengths in [0, 40], no leading zeros, no duplicate keys, nothing truncated or trailing.
// Keys / values may be empty and may contain ':' or ';'.

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <cctype>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::to_string;
using std::isdigit;

// Length-prefix every string, so the decoder never looks for a delimiter inside data.
// T: O(L log p) for L total characters and p pairs, S: O(L).
string serialize(const vector<string>& flat) {
    // sorted[key] = value; map orders keys by ASCII
    map<string, string> sorted;
    for (int i = 0; i + 1 < (int)flat.size(); i += 2) {
        sorted[flat[i]] = flat[i + 1];
    }
    string out = to_string(sorted.size()) + ";";
    for (const auto& [key, value] : sorted) {
        out += to_string(key.size()) + ":" + key;
        out += to_string(value.size()) + ":" + value;
    }
    return out;
}

// Read a decimal in [0, limit] at pos followed by sep; pos ends after sep. -1 if invalid.
int readNumber(const string& s, int& pos, int limit, char sep) {
    int start = pos;
    int value = 0;
    while (pos < (int)s.size() && isdigit(s[pos])) {
        value = value * 10 + (s[pos] - '0');
        pos++;
        // check inside the loop: a long digit run can't overflow
        if (value > limit) {
            return -1;
        }
    }
    int digits = pos - start;
    if (digits == 0 || (digits > 1 && s[start] == '0')) {
        return -1;
    }
    if (pos >= (int)s.size() || s[pos] != sep) {
        return -1;
    }
    pos++;
    return value;
}

// Read "length:" then exactly length characters into field.
bool readField(const string& s, int& pos, string& field) {
    int length = readNumber(s, pos, 40, ':');
    if (length == -1 || pos + length > (int)s.size()) {
        return false;
    }
    field = s.substr(pos, length);
    pos += length;
    return true;
}

// One left-to-right scan with a cursor; every check fails fast. T: O(L log N), S: O(L).
vector<string> deserialize(const string& s) {
    const vector<string> error = {"ERROR"};
    // cursor into s
    int pos = 0;
    int n = readNumber(s, pos, 20, ';');
    if (n == -1) {
        return error;
    }

    // store[key] = value, kept sorted by key
    map<string, string> store;
    for (int i = 0; i < n; i++) {
        string key;
        string value;
        bool ok = readField(s, pos, key) && readField(s, pos, value);
        if (!ok || store.count(key)) {
            return error;
        }
        store[key] = value;
    }

    // nothing may follow the last record
    if (pos != (int)s.size()) {
        return error;
    }

    vector<string> flat;
    for (const auto& [key, value] : store) {
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
    test("bad-record", {"deserialize", "deserialize", "deserialize", "deserialize",
                        "deserialize", "deserialize"},
         {{"1;1:a"}, {"1;01:a1:x"}, {"1;1a1:x"}, {"1;41:a1:x"}, {"1;1:a1:x;"},
          {"2;1:a1:x1:a1:y"}},
         {{"ERROR"}, {"ERROR"}, {"ERROR"}, {"ERROR"}, {"ERROR"}, {"ERROR"}});
}

// follow-up: why not escape ':' / ';' -> works too, but a length prefix skips / copies a
//   field without scanning it and has no unescape pass
// follow-up: binary / large data -> fixed 4-byte big-endian length or varint (7 bits per
//   byte, high bit = more), as in protobuf
// follow-up: no small length limit -> reject when value > (INT_MAX - d) / 10 before
//   value = value * 10 + d; check length > size - pos (no overflow)
// follow-up: format evolution / corruption -> version byte in header + CRC32 checksum
