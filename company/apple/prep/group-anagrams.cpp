// group the strings being anagrams of each other

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

// sorted str as key, T: O(nklogk), S: O(nk), k = max word len
vector<vector<string>> groupAnagrams(const vector<string>& strs) {
    unordered_map<string, vector<string>> groups; // key -> groups of str
    for (const auto& s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        groups[key].push_back(s);
    }

    vector<vector<string>> result;
    for (auto& [key, words] : groups) { result.push_back(words); }
    return result;
}

// char-count array as key, T: O(nk), S: O(nk)
vector<vector<string>> groupAnagramsCount(const vector<string>& strs) {
    unordered_map<string, vector<string>> groups; // count-key -> groups of str
    for (const auto& s : strs) {
        string key(26, 0); // key[i] = count of letter
        for (char c : s) { key[c - 'a']++; }
        groups[key].push_back(s);
    }

    vector<vector<string>> result;
    for (auto& [key, words] : groups) { result.push_back(words); }
    return result;
}

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
    test("ex1", {"eat", "tea", "tan", "ate", "nat", "bat"},
         {{"bat"}, {"nat", "tan"}, {"ate", "eat", "tea"}});
    test("empty", {""}, {{""}});
    test("single", {"a"}, {{"a"}});
}
