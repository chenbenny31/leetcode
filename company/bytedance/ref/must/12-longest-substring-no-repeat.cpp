// ByteDance (hack2hire) / LC 3: length of the longest substring of s without
// repeating characters.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Sliding window; on a repeat, jump left past that char's last index.
// T: O(n), S: O(1) (256 slots).
int lengthOfLongestSubstring(const string& s) {
    // last[c] = latest index of char c, -1 = not seen yet
    vector<int> last(256, -1);
    int best = 0;
    // window = s[left..i], no repeats inside
    int left = 0;
    int n = s.size();
    for (int i = 0; i < n; i++) {
        unsigned char c = s[i];
        // max: a last index outside the window must not move left backwards
        left = max(left, last[c] + 1);
        last[c] = i;
        best = max(best, i - left + 1);
    }
    return best;
}

void test(const string& label, const string& s, int want) {
    cout << label << (lengthOfLongestSubstring(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("repeat-later", "abcabcbb", 3);
    test("all-same", "bbbbb", 1);
    test("stale-index", "abba", 2);
    test("empty", "", 0);
}

// follow-up: return the substring -> record start when best improves
//   if (i - left + 1 > best) { best = i - left + 1; bestStart = left; }
//   return s.substr(bestStart, best);
// follow-up: at most k distinct chars (LC 340) -> count[c] + distinct; shrink left one
//   step at a time while distinct > k (can't jump), O(n)
// follow-up: int elements / large alphabet -> unordered_map<int, int> last, same logic
// follow-up: min window substring (LC 76) -> need[c] + missing; expand right until
//   missing == 0, shrink left while valid, track best, O(n + m)
