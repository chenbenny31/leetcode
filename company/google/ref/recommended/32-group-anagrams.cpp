// 32 · Group Anagrams — LC 49, 249, 242
// Mother problem: select/01-arrays-hashing/03-group-anagrams.cpp
//
// Google reports: group shifted ×3, string bag, anagram subsequences
//
// LC 49: group the lowercase strings that are anagrams of each other; any order of
// groups and of words within a group.
//
// Clarify
// - n up to 1e4 words of length up to 100, lowercase → sorting each word is cheap
// - empty strings → one group; duplicate words → each copy stays, in the same group
//
// Core idea
// - Anagrams share one canonical key: the sorted letters of a copy of the word.
// - One hash map from key to words builds every group in one pass.
// - T O(n * k log k), S O(n * k); k = max word length.
//
// Tests
// - two groups plus a single; same letters with different counts; empty words
//
// Follow-ups
// - no sort → key = the 26 letter counts joined with '#' ("1#11#" vs "11#1#"), O(n * k)
// - group shifted strings (LC 249) → key = adjacent diffs (word[i] − word[i−1] + 26) % 26
// - all anagram starts of p in s (LC 438) → fixed window of |p| letter counts, compare
//   with p's counts at each step
// - valid anagram (LC 242) → one count array, + for s and − for t, all must end at 0
// - any charset → sorted key unchanged; the count key becomes a map

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
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {"ab", "c", "ba"}, {{"ab", "ba"}, {"c"}});
    test("same-letters-diff-count", {"aab", "ab"}, {{"aab"}, {"ab"}});
    test("empty-word", {"", "b", ""}, {{"", ""}, {"b"}});
}
