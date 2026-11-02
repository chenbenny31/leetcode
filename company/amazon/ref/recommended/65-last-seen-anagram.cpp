// Last seen anagram: for each lowercase word in order, output the most recent earlier
// word that is an anagram of it (an exact copy counts), or the word itself if none.
// Then remember it as the newest word of its anagram group.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::to_string;

// key = the 26 letter counts joined with '#', so anagrams share a key without sorting
string letterCountKey(const string& word) {
    vector<int> count(26, 0);
    for (char c : word) { count[c - 'a']++; }
    string key;
    for (int x : count) { key += to_string(x) + "#"; }
    return key;
}

// Map from letter-count key to the newest word with that key. T: O(total chars),
// S: O(total chars).
vector<string> lastSeenAnagrams(const vector<string>& words) {
    // newest[key] = most recent word with that letter-count key
    unordered_map<string, string> newest;
    vector<string> result;
    for (const string& w : words) {
        string key = letterCountKey(w);
        auto it = newest.find(key);
        if (it == newest.end()) {
            result.push_back(w);
        } else {
            result.push_back(it->second);
        }
        newest[key] = w;
    }
    return result;
}

void test(const string& label, const vector<string>& words, const vector<string>& want) {
    cout << label << (lastSeenAnagrams(words) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("newest-match", {"ab", "ba", "ab"}, {"ab", "ab", "ba"});
    test("no-match", {"x", "y"}, {"x", "y"});
    test("length-differs", {"a", "aa"}, {"a", "aa"});
}
