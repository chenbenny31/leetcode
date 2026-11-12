// LC 49 Group Anagrams (Medium). Pattern: hash map with canonical key.
// Group the lowercase strings that are anagrams of each other; any order of groups and
// of words within a group.

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
// shift does not change. T: O(n * k), S: O(n * k).
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

int main() {
    test("basic", {"ab", "c", "ba"}, {{"ab", "ba"}, {"c"}});
    test("same-letters-diff-count", {"aab", "ab"}, {{"aab"}, {"ab"}});
    test("empty-word", {"", "b", ""}, {{"", ""}, {"b"}});
    testShifted("shift-basic", {"abc", "bcd", "acd"}, {{"abc", "bcd"}, {"acd"}});
    testShifted("shift-wrap", {"ab", "za", "ba"}, {{"ab", "za"}, {"ba"}});
    testShifted("shift-single-letters", {"a", "z", "ab"}, {{"a", "z"}, {"ab"}});
}

// follow-up: all anagram starts of p in s (LC 438) -> fixed window |p| with letter
//   counts, compare counts each step, O(26 n)
// follow-up: any charset -> sorted key still works; count key becomes map<char, int>
