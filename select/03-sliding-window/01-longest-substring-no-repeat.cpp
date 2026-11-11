// LC 3 Longest Substring Without Repeating Characters (Medium). Pattern: variable
// window, jump left past last seen index.
// Return the length of the longest substring of s (ASCII) with no repeated character.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Window s[left..right] has no repeats; on a repeat, jump left past that char's last
// index. T: O(n), S: O(1) (128 slots).
int lengthOfLongestSubstring(const string& s) {
    // last[c] = latest index of char c, -1 = not seen yet
    vector<int> last(128, -1);
    int best = 0;
    int left = 0;
    int n = s.size();
    for (int right = 0; right < n; right++) {
        char c = s[right];
        // max: a last index left of the window must not move left backwards
        left = max(left, last[c] + 1);
        last[c] = right;
        best = max(best, right - left + 1);
    }
    return best;
}

// LC 340: window s[left..right] has at most k distinct chars; past k, shrink left one
// step at a time (can't jump). T: O(n), S: O(1) (128 slots).
int longestAtMostKDistinct(const string& s, int k) {
    // count[c] = occurrences of char c in the window
    vector<int> count(128, 0);
    // number of chars with count > 0 in the window
    int distinct = 0;
    int best = 0;
    int left = 0;
    int n = s.size();
    for (int right = 0; right < n; right++) {
        char c = s[right];
        if (count[c] == 0) {
            distinct++;
        }
        count[c]++;
        while (distinct > k) {
            char out = s[left];
            count[out]--;
            if (count[out] == 0) {
                distinct--;
            }
            left++;
        }
        best = max(best, right - left + 1);
    }
    return best;
}

void test(const string& label, const string& s, int want) {
    cout << label << (lengthOfLongestSubstring(s) == want ? " PASS\n" : " FAIL\n");
}

void testAtMostK(const string& label, const string& s, int k, int want) {
    cout << label << (longestAtMostKDistinct(s, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("repeat-at-start", "abcad", 4);
    test("all-same", "aaa", 1);
    test("stale-index", "abba", 2);
    test("empty", "", 0);
    testAtMostK("k-shrink-two", "abaccc", 2, 4);
    testAtMostK("k-one", "abbbcc", 1, 3);
    testAtMostK("k-zero", "abc", 0, 0);
    testAtMostK("k-above-distinct", "aab", 5, 3);
}

// follow-up: return the substring -> record bestStart = left whenever best improves
// follow-up: large alphabet / ints -> unordered_map<int, int> last, same logic
