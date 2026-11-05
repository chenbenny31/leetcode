// ByteDance (hack2hire) / LC 424: s of uppercase letters; change at most k chars to any
// letter. Return the length of the longest substring of one repeated letter.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Sliding window; valid while len - maxFreq <= k. T: O(n), S: O(1).
int characterReplacement(const string& s, int k) {
    // cnt[c] = count of letter c in the window s[left..i]
    vector<int> cnt(26, 0);
    // highest single-letter count any window has reached
    int maxFreq = 0;
    int best = 0;
    int left = 0;
    int n = s.size();
    for (int i = 0; i < n; i++) {
        cnt[s[i] - 'A']++;
        maxFreq = max(maxFreq, cnt[s[i] - 'A']);
        // maxFreq is never decreased: only a larger maxFreq can beat best
        while (i - left + 1 - maxFreq > k) {
            cnt[s[left] - 'A']--;
            left++;
        }
        best = max(best, i - left + 1);
    }
    return best;
}

void test(const string& label, const string& s, int k, int want) {
    cout << label << (characterReplacement(s, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", "ABAB", 2, 4);
    test("lc-ex2", "AABABBA", 1, 4);
    test("single", "A", 0, 1);
}

// follow-up: why maxFreq never decreases -> a stale maxFreq only keeps an old valid
//   window length; best grows only when a real larger maxFreq appears
// follow-up: return the substring -> bestStart = left whenever best improves
// follow-up: binary array, flip <= k zeros (LC 1004) -> count zeros in window, shrink
//   left while zeros > k, O(n)
