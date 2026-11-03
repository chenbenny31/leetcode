// substr or subseq? substr, be contiguous
// char set? ASCII, 256 is enough
// case-sensitive: yes
// return? len, substr is follow-up
// be empty? yes

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// sliding-window s[left..i], on repeat, jump left past char's last idx
// T: O(n), S: O(1), 256 chars
int lengthOfLongestSubstring(const string& s) {
    vector<int> last(256, -1);
    int best = 0;
    int left = 0; // win = s[left..i], no repeat inside
    int n = s.size();
    for (int i = 0; i < n; i++) {
        unsigned char c = s[i];
        left = max(left, last[c] + 1); // skip by lastIdx of c
        last[c] = i;
        best = max(best, i - left + 1);
    }
    return best;
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
