// LC 49: group the strings that are anagrams of each other; any order of groups
// and of words within a group.
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

// straightforward: key = the word's letters sorted; k = max word length.
// T: O(n * k log k), S: O(n * k).
vector<vector<string>> groupAnagrams(const vector<string>& strs) {
    unordered_map<string, vector<string>> groups;   // sorted letters -> words
    for (const auto& s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        groups[key].push_back(s);
    }
    vector<vector<string>> result;
    for (auto& [key, words] : groups) { result.push_back(words); }
    return result;
}

// improved: key = the 26 letter counts, one byte each (lowercase only, counts < 256);
// no sort, and the key is a fixed 26 bytes. T: O(n * k), S: O(n * k).
vector<vector<string>> groupAnagramsCount(const vector<string>& strs) {
    unordered_map<string, vector<string>> groups;   // count key -> words
    for (const auto& s : strs) {
        // key[i] = count of letter 'a' + i
        string key(26, 0);
        for (char c : s) { key[c - 'a']++; }
        groups[key].push_back(s);
    }
    vector<vector<string>> result;
    for (auto& [key, words] : groups) { result.push_back(words); }
    return result;
}

// canonical form for comparing: sort words inside each group, then sort the groups
vector<vector<string>> canon(vector<vector<string>> groups) {
    for (auto& g : groups) { sort(g.begin(), g.end()); }
    sort(groups.begin(), groups.end());
    return groups;
}

void test(const string& label, const vector<string>& strs,
          const vector<vector<string>>& want) {
    bool ok = canon(groupAnagrams(strs)) == canon(want) &&
              canon(groupAnagramsCount(strs)) == canon(want);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {"ab", "ba", "c"}, {{"ab", "ba"}, {"c"}});
    test("counts", {"aab", "aba", "abb"}, {{"aab", "aba"}, {"abb"}});
    test("empty", {""}, {{""}});
}
