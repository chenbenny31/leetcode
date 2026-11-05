// ByteDance (hack2hire) / LC 49: group the strings that are anagrams of each other;
// any order of groups and of words within a group.
// Follow-up: avoid the per-word sort with a letter-count key.

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
    // sorted letters -> words
    unordered_map<string, vector<string>> groups;
    for (const string& s : strs) {
        string key = s;
        // sort(key.begin(), key.end());
        groups[key].push_back(s);
    }
    vector<vector<string>> result;
    for (auto& [key, words] : groups) {
        result.push_back(words);
    }
    return result;
}

// Follow-up: key = 26 letter counts joined with '#'. T: O(n * k), S: O(n * k).
vector<vector<string>> groupAnagramsCount(const vector<string>& strs) {
    // count key -> words
    unordered_map<string, vector<string>> groups;
    for (const string& s : strs) {
        // cnt[i] = occurrences of letter 'a' + i
        vector<int> cnt(26, 0);
        for (char c : s) {
            cnt[c - 'a']++;
        }
        string key;
        // separator keeps counts 1,11 distinct from 11,1
        for (int x : cnt) {
            key += to_string(x) + '#';
        }
        groups[key].push_back(s);
    }
    vector<vector<string>> result;
    for (auto& [key, words] : groups) {
        result.push_back(words);
    }
    return result;
}

// canonical form: sort words inside each group, then sort the groups
vector<vector<string>> normalize(vector<vector<string>> groups) {
    for (vector<string>& g : groups) {
        sort(g.begin(), g.end());
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

int main() {
    test("basic", {"ab", "c", "ba"}, {{"ab", "ba"}, {"c"}});
    test("same-letters", {"aab", "ab"}, {{"aab"}, {"ab"}});
    test("empty-word", {"", "b"}, {{""}, {"b"}});
}

// follow-up: avoid per-word sort -> see groupAnagramsCount() above; 26 counts joined
//   with '#' as key, O(n * k)
// follow-up: all anagram starts of p in s (LC 438) -> fixed window |p|, diff[26] +
//   matches counter, O(n)
// follow-up: any charset -> sorted key still works; count key becomes a map<char, int>
//   serialized in key order
