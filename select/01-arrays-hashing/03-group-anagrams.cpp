// LC 49 Group Anagrams (Medium) — hash map with a canonical key
// group strings that are anagrams of each other; any order of groups and of words
//
// clarify:
// charset -> lowercase only; sorted key works for any charset
// empty strings -> one group together
// dup words -> each copy stays
// output order -> any

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

// core idea: anagrams share one key = the word's letters sorted; map key -> words
// T: O(n * k log k), S: O(n * k) — k = word len
vector<vector<string>> groupAnagrams(const vector<string>& strs) {
    // groups[sorted letters] = words w/ those letters
    unordered_map<string, vector<string>> groups;
    for (const string& word : strs) {
        // sort a copy; the group stores the original word
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

// follow-up:
// no sort, O(n * k) -> key = 26 letter cnts joined w/ a separator (else 1,11 = 11,1)
// any charset -> sorted key unchanged; cnt key becomes ordered (char, cnt) pairs
// group shifted strings (LC 249) -> key = (word[i] - word[i - 1] + 26) % 26 per pair
// all anagram starts of p in s (LC 438) -> slide window of |p|, compare 26 cnts
// valid anagram (LC 242) -> equal sizes, one cnt array: + for s, - for t, all end 0
