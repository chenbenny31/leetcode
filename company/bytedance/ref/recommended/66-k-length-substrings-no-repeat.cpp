// ByteDance (hack2hire) / LC 1100: count the substrings of s (lowercase) of length k
// that have no repeated characters.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Fixed window of size k, tracking how many letters repeat inside. T: O(n), S: O(1).
int numKLenSubstrNoRepeats(const string& s, int k) {
    // cnt[c] = occurrences of letter c in the window
    vector<int> cnt(26, 0);
    // letters with cnt >= 2 in the window
    int dup = 0;
    int result = 0;
    int n = s.size();
    for (int i = 0; i < n; i++) {
        cnt[s[i] - 'a']++;
        if (cnt[s[i] - 'a'] == 2) {
            dup++;
        }
        if (i >= k) {
            cnt[s[i - k] - 'a']--;
            // the outgoing letter just stopped repeating
            if (cnt[s[i - k] - 'a'] == 1) {
                dup--;
            }
        }
        if (i >= k - 1 && dup == 0) {
            result++;
        }
    }
    return result;
}

void test(const string& label, const string& s, int k, int want) {
    cout << label << (numKLenSubstrNoRepeats(s, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", "havefunonleetcode", 5, 6);
    test("repeat-inside", "aab", 2, 1);
    test("k->-n", "home", 5, 0);
}

// follow-up: count repeat-free substrings of any length -> per right,
//   left = max(left, last[c] + 1), add right - left + 1; O(n)
// follow-up: general charset -> unordered_map<char, int> cnt (or int cnt[256]);
//   k > alphabet size can return 0 right away (pigeonhole)
// follow-up: distinct such substrings -> unordered_set<string> of s.substr(i - k + 1, k),
//   O(n * k)
