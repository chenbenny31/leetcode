// LC 49 Group Anagrams (Medium) — hash map with a canonical key
// Group the strings that are anagrams of each other; any order of groups and of words
// inside a group.
// Clarify: lowercase letters only; empty strings (one group together); duplicate words
//   (each copy stays); output order; word length.

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

// Core idea: anagrams share one canonical key, the word's letters sorted; a map from key
// to words builds every group in one pass. k = word length.
// T: O(n * k log k), S: O(n * k).
vector<vector<string>> groupAnagrams(const vector<string>& strs) {
    // sorted letters -> words with those letters
    unordered_map<string, vector<string>> groups;
    for (const string& word : strs) {
        // sort a copy: the group stores the original word
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
    return 0;
}

// follow-up (ideas only):
// - no sort, O(n * k) -> key = the 26 letter counts joined with a separator (without it
//   counts 1,11 and 11,1 both read "111")
// - any charset -> the sorted key works unchanged; the count key becomes ordered
//   (char, count) pairs
// - group shifted strings (LC 249) -> key = adjacent letter differences,
//   (word[i] - word[i - 1] + 26) % 26, which a shift does not change
// - all anagram starts of p in s (LC 438) -> slide a window of |p| letters, keep its 26
//   counts and compare with p's counts once the window is full
// - valid anagram (LC 242) -> equal sizes, one count array: + for s, - for t, all end 0
