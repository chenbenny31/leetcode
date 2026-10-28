// Shift every letter of s by shift positions (any integer, may be negative or large),
// wrapping within A-Z / a-z and keeping case; other characters stay the same.
// Follow-up: for long strings and a fixed shift, use a lookup table.

#include <iostream>
#include <string>
#include <cctype>

using std::cout;
using std::string;
using std::isupper;
using std::islower;

// straightforward: compute each shifted letter. T: O(n), S: O(n) output.
string caesar(const string& s, int shift) {
    int k = ((shift % 26) + 26) % 26;
    string result = s;
    for (char& c : result) {
        if (isupper(c)) {
            c = 'A' + (c - 'A' + k) % 26;
        } else if (islower(c)) {
            c = 'a' + (c - 'a' + k) % 26;
        }
    }
    return result;
}

// improved: map each of the 256 byte values once, then one table lookup per character.
// T: O(256 + n), S: O(256).
string caesarTable(const string& s, int shift) {
    int k = ((shift % 26) + 26) % 26;
    // table[c] = what byte c turns into
    char table[256];
    for (int c = 0; c < 256; c++) { table[c] = (char)c; }
    for (int i = 0; i < 26; i++) {
        table['A' + i] = 'A' + (i + k) % 26;
        table['a' + i] = 'a' + (i + k) % 26;
    }
    string result = s;
    for (char& c : result) { c = table[(unsigned char)c]; }
    return result;
}

void test(const string& label, const string& s, int shift, const string& want) {
    bool ok = caesar(s, shift) == want && caesarTable(s, shift) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("wrap", "xyz", 3, "abc");
    test("case", "AbC", 1, "BcD");
    test("punct", "a-b!", 1, "b-c!");
    test("negative", "abc", -1, "zab");
    test("big", "a", 53, "b");
}
