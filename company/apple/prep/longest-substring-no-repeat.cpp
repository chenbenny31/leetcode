// return length of the longest substring os s without repeating chars

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// sliding-window, on repeat: jump left past cur idx, T: O(n), S: O(1)
int lengthOfLongestSubstring(const string& s) {
    const int n = s.size();
    vector<int> last(256, -1); // last[c] = last-idx of char c, -1 = not seen
    int maxLen = 0; // win = s[left..i]
    int left = 0;

    for (int right = 0; right < n; right++) {
        unsigned char c = s[right];
        left = max(left, last[c] + 1); // jump to last repeat char idx + 1
        last[c] = right;
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}

void test(const string& label, const string& s, int want) {
    cout << label << (lengthOfLongestSubstring(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "abcabcbb", 3);
    test("all same", "bbbbb", 1);
    test("stale index", "abba", 2);
    test("empty", "", 0);
}
