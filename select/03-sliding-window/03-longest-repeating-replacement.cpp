// LC 424 Longest Repeating Character Replacement (Medium). Pattern: variable window,
// len - maxFreq <= k.
// s of uppercase letters; change at most k chars to any letter. Return the length of
// the longest substring of one repeated letter.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Window is valid while its non-majority chars (len - maxFreq) fit in k.
// T: O(n), S: O(1).
int characterReplacement(const string& s, int k) {
    // count[c] = count of letter 'A' + c in the window s[left..right]
    vector<int> count(26, 0);
    // highest single-letter count any window has reached
    int maxFreq = 0;
    int best = 0;
    int left = 0;
    int n = s.size();
    for (int right = 0; right < n; right++) {
        int letter = s[right] - 'A';
        count[letter]++;
        maxFreq = max(maxFreq, count[letter]);
        // maxFreq is never decreased: only a larger maxFreq can beat best
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
}

// follow-up: why a stale maxFreq is fine -> it only keeps an old valid window length;
//   best grows only when a real larger maxFreq appears
// follow-up: binary array, flip <= k zeros (LC 1004) -> count zeros in window, shrink
//   left while zeros > k, O(n)
