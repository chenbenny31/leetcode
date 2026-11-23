// 44 · Validate IP Address (Cisco OA, Medium) — LC 468, char-loop parsing
// return "IPv4", "IPv6" or "Neither"
// IPv4: 4 parts split by '.', each 0..255 in decimal, no leading zero
// IPv6: 8 parts split by ':', each 1..4 hex digits, leading zeros fine, no "::"
//
// clarify:
// leading zero in IPv4 ("01") -> invalid; "0" alone is valid
// empty part ("1..2.3", trailing '.') -> invalid
// IPv6 compression "::" -> not accepted here (LC 468 rule)
// spaces, port, prefix len -> not accepted

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// keeps empty parts, so "1..2" and a trailing sep are visible to the caller
// T: O(len), S: O(len)
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

bool isV4Part(const string& part) {
    int len = part.size();
    if (len == 0 || len > 3) { return false; }
    if (len > 1 && part[0] == '0') { return false; }
    int value = 0;
    for (char c : part) {
        if (c < '0' || c > '9') { return false; }
        value = value * 10 + (c - '0');
    }
    return value <= 255;
}

bool isV6Part(const string& part) {
    int len = part.size();
    if (len == 0 || len > 4) { return false; }
    for (char c : part) {
        bool digit = c >= '0' && c <= '9';
        bool lower = c >= 'a' && c <= 'f';
        bool upper = c >= 'A' && c <= 'F';
        if (!digit && !lower && !upper) { return false; }
    }
    return true;
}

// core idea: split on the separator, count the parts, validate each part on its own
// T: O(len), S: O(len)
string validIPAddress(const string& ip) {
    vector<string> v4 = split(ip, '.');
    if (v4.size() == 4) {
        for (const string& part : v4) {
            if (!isV4Part(part)) { return "Neither"; }
        }
        return "IPv4";
    }
    vector<string> v6 = split(ip, ':');
    if (v6.size() == 8) {
        for (const string& part : v6) {
            if (!isV6Part(part)) { return "Neither"; }
        }
        return "IPv6";
    }
    return "Neither";
}

void test(const string& label, const string& ip, const string& want) {
    cout << label << (validIPAddress(ip) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("v4", "172.16.254.1", "IPv4");
    test("v6", "2001:0db8:85a3:0:0:8A2E:0370:7334", "IPv6");
    test("v4-leading-zero", "192.168.01.1", "Neither");
    test("v4-out-of-range", "256.1.1.1", "Neither");
    return 0;
}

// follow-up:
// 1. IPv4 to uint32 -> value = (value << 8) | part, for the 4 parts in order
// 2. ip in a.b.c.d/len -> mask = ~0u << (32 - len) (len 0: 0), compare ip & mask
// 3. longest prefix match over CIDRs -> binary trie, company/cloudflare/ref/05
// 4. accept "::" -> at most one empty run, standing for the missing zero groups
// 5. all valid IPs from a digit str (LC 93) -> select/optional/12-backtracking/05
