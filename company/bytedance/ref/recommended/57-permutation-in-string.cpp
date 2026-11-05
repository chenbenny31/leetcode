// ByteDance (hack2hire) / LC 567: return true if some permutation of s1 is a substring
// of s2 (both lowercase).
// Follow-up LC 438: return every start index in s of an anagram of p.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// add delta to letter ch's diff, keeping matches = letters whose diff is 0
void update(vector<int>& diff, int& matches, char ch, int delta) {
    int c = ch - 'a';
    if (diff[c] == 0) {
        matches--;
    }
    diff[c] += delta;
    if (diff[c] == 0) {
        matches++;
    }
}

// Fixed window of size |s1|; a permutation iff all 26 letters match. T: O(n), S: O(1).
bool checkInclusion(const string& s1, const string& s2) {
    int m = s1.size();
    int n = s2.size();
    if (m > n) {
        return false;
    }
    // diff[c] = count of c in s1 - count of c in window
    vector<int> diff(26, 0);
    for (int i = 0; i < m; i++) {
        diff[s1[i] - 'a']++;
        diff[s2[i] - 'a']--;
    }
    // letters with diff[c] == 0
    int matches = 0;
    for (int c = 0; c < 26; c++) {
        if (diff[c] == 0) {
            matches++;
        }
    }
    if (matches == 26) {
        return true;
    }
    for (int i = m; i < n; i++) {
        // s2[i] enters, s2[i - m] leaves
        update(diff, matches, s2[i], -1);
        update(diff, matches, s2[i - m], +1);
        if (matches == 26) {
            return true;
        }
    }
    return false;
}

// Follow-up: same window, record every start where all 26 letters match.
// T: O(n), S: O(1) besides the output.
vector<int> findAnagrams(const string& s, const string& p) {
    int m = p.size();
    int n = s.size();
    vector<int> result;
    if (m > n) {
        return result;
    }
    // diff[c] = count of c in p - count of c in window
    vector<int> diff(26, 0);
    for (int i = 0; i < m; i++) {
        diff[p[i] - 'a']++;
        diff[s[i] - 'a']--;
    }
    // letters with diff[c] == 0
    int matches = 0;
    for (int c = 0; c < 26; c++) {
        if (diff[c] == 0) {
            matches++;
        }
    }
    if (matches == 26) {
        result.push_back(0);
    }
    for (int i = m; i < n; i++) {
        update(diff, matches, s[i], -1);
        update(diff, matches, s[i - m], +1);
        if (matches == 26) {
            result.push_back(i - m + 1);
        }
    }
    return result;
}

void test(const string& label, const string& s1, const string& s2, bool want) {
    cout << label << (checkInclusion(s1, s2) == want ? " PASS\n" : " FAIL\n");
}

void testAll(const string& label, const string& s, const string& p,
             const vector<int>& want) {
    cout << label << (findAnagrams(s, p) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", "ab", "xbay", true);
    test("at-end", "abc", "xxcab", true);
    test("count-differs", "aab", "abba", false);
    test("too-long", "abc", "ab", false);
    testAll("all-starts", "abab", "ab", {0, 1, 2});
}

// follow-up: all start indices (LC 438) -> see findAnagrams() above; push i - m + 1
//   whenever matches == 26
// follow-up: any charset -> diff in unordered_map<char, int> (or int[256]); track
//   nonZero = keys with diff != 0, permutation iff nonZero == 0
// follow-up: min window containing all of t (LC 76) -> variable window: need[c] +
//   missing; expand right, shrink left while missing == 0, O(m + n)
