// 36 · Group Anagrams — LC 49, 249, 242
// Mother: select/01-arrays-hashing/03-group-anagrams.cpp
// LC 49: group lowercase strings that are anagrams; any order of groups and words
// reported: group shifted ×3, string bag, anagram subsequences
//
// clarify:
// sizes -> 1e4 words, len <= 100
// empty strings -> one group
// dup words -> each copy stays
// charset -> lowercase a-z; other charsets in follow-up

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

// core idea: anagrams share one key = sorted letters; map key -> words
// T: O(n * k log k), S: O(n * k) — k = max word len
vector<vector<string>> groupAnagrams(const vector<string>& strs) {
    // sorted letters -> words w/ those letters
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

// canonical form for comparing: sort words in each group, then sort groups
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

// follow-up:
// no sort -> key = 26 letter cnts joined w/ '#' ("1#11#" vs "11#1#"); O(n * k)
// group shifted strings (LC 249) -> key = adjacent diffs (w[i] - w[i - 1] + 26) % 26
// all anagram starts of p in s (LC 438) -> fixed window of |p| letter cnts vs p's cnts
// valid anagram (LC 242) -> one cnt array, + for s, - for t, all must end at 0
// any charset -> sorted key unchanged; cnt key becomes a map
