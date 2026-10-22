// LC 443 String Compression: compress chars in place by replacing each run of a repeated
// character with the character plus the run length when the run is longer than 1.
// Return the new length, using only O(1) extra space.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::to_string;
using std::vector;

// Two pointers: a read pointer finds each run, a write pointer (never ahead of it) emits
// the char and its count digits. T: O(n), S: O(1).
int compress(vector<char>& chars) {
    int n = chars.size(), w = 0, i = 0;   // w = next write position
    while (i < n) {
        int j = i;
        while (j < n && chars[j] == chars[i]) {
            j++;
        }
        chars[w] = chars[i];
        w++;
        if (j - i > 1) {
            for (char d : to_string(j - i)) {
                chars[w] = d;
                w++;
            }
        }
        i = j;
    }
    return w;
}

void test(const string& label, const string& s, const string& want) {
    vector<char> chars(s.begin(), s.end());
    int len = compress(chars);
    string got(chars.begin(), chars.begin() + len);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("runs", "aabbccc", "a2b2c3");
    test("no-runs", "abc", "abc");
    test("multi-digit", "bbbbbbbbbbbb", "b12");
    test("single-char", "z", "z");
}
