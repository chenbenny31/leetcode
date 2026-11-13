// LC 424 Longest Repeating Character Replacement (Medium) — window, len - maxFreq <= k
// change at most k letters of s; return len of longest substring of one repeated letter
//
// clarify:
// charset -> uppercase only, 26 slots
// k = 0 or k >= n -> ok, same code
// empty string -> 0
// return -> len, not the substring

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// core idea: window valid while len - maxFreq <= k (letters to change); else shrink left
// T: O(n), S: O(1)
int characterReplacement(const string& s, int k) {
    // count[c] = cnt of letter 'A' + c in window s[left..right]
    vector<int> count(26, 0);
    // maxFreq = highest single-letter cnt any window has reached
    int maxFreq = 0;
    int best = 0;
    int left = 0;
    int n = s.size();
    for (int right = 0; right < n; right++) {
        int letter = s[right] - 'A';
        count[letter]++;
        maxFreq = max(maxFreq, count[letter]);
        // maxFreq never decreased: only a larger maxFreq can beat best
        while (right - left + 1 - maxFreq > k) {
            count[s[left] - 'A']--;
            left++;
        }
        best = max(best, right - left + 1);
    }
    return best;
}

void test(const string& label, const string& s, int k, int want) {
    cout << label << (characterReplacement(s, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("no-change", "AABA", 0, 2);
    test("one-change", "ABAA", 1, 4);
    test("spread", "ABCAB", 2, 4);
    test("single", "B", 0, 1);
    return 0;
}

// follow-up:
// why stale maxFreq is fine -> it only keeps an old valid len; best needs a larger one
// binary array, flip at most k zeros (LC 1004) -> cnt zeros in window, shrink if > k
