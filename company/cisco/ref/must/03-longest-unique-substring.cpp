// 03 · Longest Substring Without Repeats, Return It (Cisco embedded live, Medium) — LC 3
// Mother: select/03-sliding-window/01-longest-substring-no-repeat.cpp
// return the longest substring of s with no repeated char (not just its len)
//
// clarify:
// several of the max len -> return the first one (any is accepted)
// chars -> ASCII (letters, digits, symbols, spaces), 128 slots
// empty str -> ""
// len -> up to 5e4, so O(n)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: window [left, right] has no repeats; when s[right] was seen inside the
// window, jump left past its last idx; remember where the best window starts
// T: O(n), S: O(1) — 128 slots
string longestUniqueSubstring(const string& s) {
    int n = s.size();
    // last[c] = last idx where char c was seen, -1 if never
    vector<int> last(128, -1);
    int left = 0;
    int bestStart = 0;
    int bestLen = 0;
    for (int right = 0; right < n; right++) {
        int c = (unsigned char)s[right];
        // only a repeat inside the window moves left; an older one is ignored
        if (last[c] >= left) { left = last[c] + 1; }
        last[c] = right;
        if (right - left + 1 > bestLen) {
            bestLen = right - left + 1;
            bestStart = left;
        }
    }
    return s.substr(bestStart, bestLen);
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (longestUniqueSubstring(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("first-of-ties", "abcabcbb", "abc");
    test("middle", "pwwkew", "wke");
    test("stale-index", "abba", "ab");
    test("empty", "", "");
    return 0;
}

// follow-up:
// 1. C version -> int last[128] filled with -1; return start + len, not a str
// 2. bytes > 127 -> the (unsigned char) cast keeps the idx in 0..255 (use 256 slots)
// 3. at most k distinct -> company/cisco/ref/recommended/20-longest-subarray-k-distinct
