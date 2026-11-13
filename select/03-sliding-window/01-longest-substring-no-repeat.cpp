// LC 3 Longest Substring Without Repeating Characters (Medium) — window, jump left
// return len of longest substring of s w/ no repeated char
//
// clarify:
// charset -> ASCII, 128 slots; bigger = follow-up
// substring -> contiguous
// space -> a normal char
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

// core idea: s[left..right] has no repeat; on a repeat jump left past its last idx
// T: O(n), S: O(1) — 128 slots
int lengthOfLongestSubstring(const string& s) {
    // last[c] = latest idx of char c, -1 = not seen
    vector<int> last(128, -1);
    int best = 0;
    int left = 0;
    int n = s.size();
    for (int right = 0; right < n; right++) {
        char c = s[right];
        // max: a last idx left of the window must not move left backwards
        left = max(left, last[c] + 1);
        last[c] = right;
        best = max(best, right - left + 1);
    }
    return best;
}

void test(const string& label, const string& s, int want) {
    cout << label << (lengthOfLongestSubstring(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("repeat-at-start", "abcad", 4);
    test("all-same", "aaa", 1);
    test("stale-index", "abba", 2);
    test("empty", "", 0);
    return 0;
}

// follow-up:
// return the substring -> record bestStart = left whenever best improves
// large alphabet or ints -> last becomes a hash map; bytes > 127 need (unsigned char)c
// at most k distinct (LC 340; LC 904 is k = 2) -> cnt per char, shrink while distinct > k
// shortest subarray w/ >= k distinct -> while distinct >= k: record len, shrink left
// exactly k distinct (LC 992) -> atMost(k) - atMost(k - 1); atMost adds right - left + 1
// O(1) "is nums[l..r] dup-free" (Google x3) -> nxt[i] = next same value; suffix min > r
