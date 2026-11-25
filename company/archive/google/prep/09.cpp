// char set -> ASCII
// empty -> 0
// space still char

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::max;

// win has no repeat, skip left with char's last idx, T: O(n), S: O(1)
int lengthOfLongestSubstring(const string& s) {
    int n = s.size();
    vector<int> last(128, -1); // last[c] = idx of char c, -1 = not seen
    int maxLen = 0;
    int left = 0; // [left, right]

    for (int right = 0; right < n; right++) {
        char c = s[right];
        left = max(left, last[c] + 1);
        last[c] = right;
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}

// extend right, while win has >= k distinct, record and shrink left
// T: O(n), S: O(n)
int shortestAtLeastKDistinct(const vector<int>& nums, int k) {
    int n = nums.size();
    if (k <= 0) { return 0; }
    unordered_map<int, int> freq; // freq[num] = its count in win
    int uniq = 0;
    int minLen = -1;
    int left = 0;

    for (int right = 0; right < n; right++) {
        int num = nums[right];
        if (freq[num] == 0) { uniq++; }
        freq[num]++;

        while (uniq >= k) {
            int len = right - left + 1;
            if (minLen == -1 || len < minLen) { minLen = len; }

            int drop = nums[left];
            freq[drop]--;
            if (freq[drop] == 0) { uniq--; }
            left++;
        }
    }
    return minLen;
}

void test(const string& label, const string& s, int want) {
    cout << label << (lengthOfLongestSubstring(s) == want ? " PASS\n" : " FAIL\n");
}

void testAtLeastK(const string& label, const vector<int>& nums, int k, int want) {
    bool ok = shortestAtLeastKDistinct(nums, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("repeat-at-start", "abcad", 4);
    test("all-same", "aaa", 1);
    test("stale-index", "abba", 2);
    test("empty", "", 0);
    testAtLeastK("at-least-later-shorter", {1, 2, 1, 1, 3, 2}, 3, 3);
    testAtLeastK("at-least-none", {2, 2, 2}, 2, -1);
    testAtLeastK("at-least-whole", {1, 2, 3}, 3, 3);
    testAtLeastK("at-least-k-zero", {4}, 0, 0);
}

