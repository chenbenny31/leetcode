// LC 49 Group Anagrams (Medium). Pattern: hash map with canonical key.
// Group the lowercase strings that are anagrams of each other; any order of groups and
// of words within a group.
//
// Clarify:
// - n up to 1e4 words of length up to 100, lowercase -> sorting each word is cheap:
//   O(n * k log k); 26 counts make it O(n * k)
// - empty strings -> allowed; they form one group together
// - duplicate words -> each copy stays in the output, in the same group
// - output order -> any order of groups and of words inside a group
// - only lowercase? -> yes; for any charset the sorted key still works, 26 counts do not
// Idea: anagrams share one canonical key, the sorted letters (or the 26 letter counts);
//   a hash map from key to words builds every group in one pass.
// Pitfalls:
// - sort a copy (string key = word); the group stores the original word
// - the count key needs a separator: counts 1,11 and 11,1 both print as "111" without one
// - index counts by c - 'a', not by c (26 slots)
// Tests to write: two groups plus a single; same letters with different counts; empty
//   words.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;
using std::to_string;

// Key = the word's letters sorted. T: O(n * k log k), S: O(n * k); k = max word length.
vector<vector<string>> groupAnagrams(const vector<string>& strs) {
    // sorted letters -> words with those letters
    unordered_map<string, vector<string>> groups;
    for (const string& word : strs) {
        string key = word;
        sort(key.begin(), key.end());
        groups[key].push_back(word);
    }
    vector<vector<string>> result;
    for (auto& [key, words] : groups) {
        result.push_back(words);
    }
    return result;
}

// Follow-up: key = 26 letter counts, no sort. T: O(n * k), S: O(n * k).
vector<vector<string>> groupAnagramsCount(const vector<string>& strs) {
    // count key -> words with those letter counts
    unordered_map<string, vector<string>> groups;
    for (const string& word : strs) {
        // letterCount[i] = occurrences of letter 'a' + i
        vector<int> letterCount(26, 0);
        for (char c : word) {
            letterCount[c - 'a']++;
        }
        string key;
        // separator keeps counts 1,11 distinct from 11,1
        for (int count : letterCount) {
            key += to_string(count) + '#';
        }
        groups[key].push_back(word);
    }
    vector<vector<string>> result;
    for (auto& [key, words] : groups) {
        result.push_back(words);
    }
    return result;
}

// LC 249 (J1) group shifted strings: key = adjacent letter differences mod 26, which a
// shift does not change.
// Clarify: shifts wrap z -> a; words of different lengths never group; all single
//   letters group together (empty key).
// Pitfall: (word[i] - word[i - 1] + 26) % 26; without + 26 "za" gets a negative diff.
// Tests to write: one shifted pair; a z -> a wrap; single letters.
// T: O(n * k), S: O(n * k).
vector<vector<string>> groupShifted(const vector<string>& strs) {
    // difference key -> words with that shifting pattern
    unordered_map<string, vector<string>> groups;
    for (const string& word : strs) {
        string key;
        for (int i = 1; i < (int)word.size(); i++) {
            // + 26 keeps a wrap like 'z' -> 'a' non-negative
            int diff = (word[i] - word[i - 1] + 26) % 26;
            key += (char)('a' + diff);
        }
        groups[key].push_back(word);
    }
    vector<vector<string>> result;
    for (auto& [key, words] : groups) {
        result.push_back(words);
    }
    return result;
}

// follow-up (LC 438): start indices of every anagram of p in s, both lowercase, p
// non-empty. Slide a window of |p| letters over s and keep its letter counts; the window
// is an anagram when its counts equal p's.
// Pitfall: drop s[i - len] once the window passes len letters, and compare only when it
// is full (i >= len - 1); the start is i - len + 1.
// T: O(26 * |s|), S: O(1).
vector<int> findAnagramStarts(const string& s, const string& p) {
    int n = s.size();
    int len = p.size();
    // pCount[c] = occurrences of letter 'a' + c in p; windowCount the same for the window
    vector<int> pCount(26, 0);
    vector<int> windowCount(26, 0);
    for (char c : p) { pCount[c - 'a']++; }
    vector<int> starts;
    for (int i = 0; i < n; i++) {
        windowCount[s[i] - 'a']++;
        if (i >= len) { windowCount[s[i - len] - 'a']--; }
        if (i >= len - 1 && windowCount == pCount) { starts.push_back(i - len + 1); }
    }
    return starts;
}

// canonical form for comparing: sort words inside each group, then sort the groups
vector<vector<string>> normalize(vector<vector<string>> groups) {
    for (vector<string>& group : groups) {
        sort(group.begin(), group.end());
    }
    sort(groups.begin(), groups.end());
    return groups;
}

void test(const string& label, const vector<string>& strs,
          const vector<vector<string>>& want) {
    bool ok = normalize(groupAnagrams(strs)) == normalize(want);
    ok = ok && normalize(groupAnagramsCount(strs)) == normalize(want);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testShifted(const string& label, const vector<string>& strs,
                 const vector<vector<string>>& want) {
    bool ok = normalize(groupShifted(strs)) == normalize(want);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testStarts(const string& label, const string& s, const string& p,
                const vector<int>& want) {
    cout << label << (findAnagramStarts(s, p) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {"ab", "c", "ba"}, {{"ab", "ba"}, {"c"}});
    test("same-letters-diff-count", {"aab", "ab"}, {{"aab"}, {"ab"}});
    test("empty-word", {"", "b", ""}, {{"", ""}, {"b"}});
    testShifted("shift-basic", {"abc", "bcd", "acd"}, {{"abc", "bcd"}, {"acd"}});
    testShifted("shift-wrap", {"ab", "za", "ba"}, {{"ab", "za"}, {"ba"}});
    testShifted("shift-single-letters", {"a", "z", "ab"}, {{"a", "z"}, {"ab"}});
    testStarts("starts-overlap", "abab", "ab", {0, 1, 2});
    testStarts("starts-counts-matter", "aab", "ab", {1});
    testStarts("starts-p-longer", "ab", "abc", {});
}

// follow-up: all anagram starts of p in s (LC 438) -> findAnagramStarts above
// follow-up: valid anagram (LC 242) -> equal sizes, then one count array, + for s and
//   - for t; every count must end at 0:
//     for (int i = 0; i < (int)s.size(); i++) {
//         letterCount[s[i] - 'a']++;
//         letterCount[t[i] - 'a']--;
//     }
// follow-up: any charset -> the sorted key works unchanged; the count key becomes a
//   map<char, int> written out as char + count pairs
