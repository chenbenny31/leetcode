// LC 1048 Longest String Chain: longest chain of words where each adds one letter to the
// previous word.
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
using std::max;

// Process words shortest first; a word's chain extends the best chain among the words
// left by deleting one of its letters. T: O(n log n + n * L^2), S: O(n * L).
int longestStrChain(vector<string> words) {
    sort(words.begin(), words.end(), [](const string& a, const string& b) {
        return a.size() < b.size();
    });

    // longest[w] = length of the longest chain ending at word w
    unordered_map<string, int> longest;
    int best = 0;
    for (const auto& w : words) {
        int len = w.size();
        int cur = 1;
        for (int i = 0; i < len; i++) {
            string pred = w.substr(0, i) + w.substr(i + 1);
            auto it = longest.find(pred);
            if (it != longest.end()) { cur = max(cur, it->second + 1); }
        }

        longest[w] = cur;
        best = max(best, cur);
    }

    return best;
}

void test(const string& label, const vector<string>& words, int want) {
    cout << label << (longestStrChain(words) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("branching", {"abcd", "abd", "a", "abc", "b", "ab"}, 4);
    test("length-gap", {"a", "abc"}, 1);
    test("two-chains", {"x", "xy", "q", "qr", "qrs"}, 3);
    test("single", {"z"}, 1);
}
