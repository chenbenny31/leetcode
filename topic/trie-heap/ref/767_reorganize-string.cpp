// LC 767 Reorganize String: rearrange the lowercase string s so no two adjacent
// characters are equal. Return any such arrangement, or "" if impossible.
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

// Greedy with a max-heap: each round take the two most frequent distinct letters, so the
// same letter is never placed twice in a row. One letter may be left over at the end;
// it fits only if it has a single copy. T: O(n log 26), S: O(26).
string reorganizeString(string s) {
    vector<int> cnt(26, 0);   // cnt[c] = occurrences of 'a' + c
    for (char ch : s) { cnt[ch - 'a']++; }
    priority_queue<pair<int, char>> heap;   // (copies left, letter), most copies on top
    for (int c = 0; c < 26; c++) {
        if (cnt[c] > 0) { heap.push({cnt[c], (char)('a' + c)}); }
    }
    string res;
    while (heap.size() >= 2) {
        auto [cntA, a] = heap.top();
        heap.pop();
        auto [cntB, b] = heap.top();
        heap.pop();
        res += a;
        res += b;
        if (cntA > 1) { heap.push({cntA - 1, a}); }
        if (cntB > 1) { heap.push({cntB - 1, b}); }
    }
    if (heap.empty()) { return res; }
    if (heap.top().first > 1) { return ""; }
    return res + heap.top().second;
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
