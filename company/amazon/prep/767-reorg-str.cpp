#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::sort;

// greedy with max-heap: each round take two most freq diff letters, T: O(nlog26), S: O(26)
string reorganizeString(string s) {
    vector<int> freq(26, 0); // freq[c] = count of 'a' + c
    for (char c : s) { freq[c - 'a']++; }

    priority_queue<pair<int, char>> heap; // max-heap, most cnt on top
    for (int i = 0; i < 26; i++) {
        if (freq[i] > 0) {
            heap.push({freq[i], (char)('a' + i)});
        }
    }

    string result;
    while (heap.size() >= 2) {
        auto [cntA, a] = heap.top();
        heap.pop();
        auto [cntB, b] = heap.top();
        heap.pop();
        result += a;
        result += b;
        if (cntA > 1) { heap.push({cntA - 1, a}); }
        if (cntB > 1) { heap.push({cntB - 1, b}); }
    }

    if (heap.empty()) { return result; }
    if (heap.top().first > 1) { return ""; }
    return result + heap.top().second;
}

void test(const string& label, const string& s, bool possible) {
    string got = reorganizeString(s);
    bool ok = true;

    if (!possible) {
        ok = got.empty();
    } else {
        string a = got, b = s;
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        ok = a == b;
        int n = got.size();
        for (int i = 1; i < n; i++) {
            if (got[i] == got[i - 1]) { ok = false; }
        }
    }

    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("possible", "aabbc", true);
    test("too-many", "aaab", false);
    test("single", "a", true);
    test("exact-half", "aaabb", true);
}

