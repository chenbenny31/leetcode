// ByteDance (FastPrep, phone / onsite) / LC 269 variant: an alien language uses lowercase
// English letters in an unknown order, and words is sorted in that order. Return the
// lexicographically smallest ordering of all letters that appear which is consistent
// with the words, or "" if none exists.
// Comparing two adjacent words: the first differing letter decides; if one word is a
// prefix of the other, the shorter must come first (otherwise the input is invalid).
// Each "a before b" relation counts once (do not add the same in-degree twice).
// (1p3a: the interviewer stressed explaining before editing, a dry run, and deduping
// edges before adding in-degree.)

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;

// Build letter edges from adjacent words, then Kahn's topological sort with a min-heap
// so the smallest available letter always goes next.
// T: O(L + 26 log 26) for L total characters, S: O(26 * 26).
string alienOrder(const vector<string>& words) {
    // present[c] = letter c appears; edge[a][b] = a must come before b
    vector<bool> present(26, false);
    vector<vector<bool>> edge(26, vector<bool>(26, false));
    vector<int> indeg(26, 0);
    for (const string& w : words) {
        for (char ch : w) {
            present[ch - 'a'] = true;
        }
    }

    for (int i = 0; i + 1 < (int)words.size(); i++) {
        const string& a = words[i];
        const string& b = words[i + 1];
        int len = a.size() < b.size() ? a.size() : b.size();
        int j = 0;
        while (j < len && a[j] == b[j]) {
            j++;
        }

        if (j == len) {
            // one is a prefix of the other: the longer one must not come first
            if (a.size() > b.size()) {
                return "";
            }
            continue;
        }

        int u = a[j] - 'a';
        int v = b[j] - 'a';
        // count each relation once
        if (!edge[u][v]) {
            edge[u][v] = true;
            indeg[v]++;
        }
    }

    // min-heap of letters whose prerequisites are all placed
    priority_queue<int, vector<int>, greater<int>> ready;
    int total = 0;
    for (int c = 0; c < 26; c++) {
        if (present[c]) {
            total++;
            if (indeg[c] == 0) {
                ready.push(c);
            }
        }
    }

    string order;
    while (!ready.empty()) {
        int cur = ready.top();
        ready.pop();
        order += (char)('a' + cur);
        for (int nxt = 0; nxt < 26; nxt++) {
            if (edge[cur][nxt]) {
                indeg[nxt]--;
                if (indeg[nxt] == 0) {
                    ready.push(nxt);
                }
            }
        }
    }

    // letters left over sit on a cycle
    if ((int)order.size() < total) {
        return "";
    }
    return order;
}

void test(const string& label, const vector<string>& words, const string& want) {
    cout << label << (alienOrder(words) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-rules", {"cb", "ca", "ab"}, "bca");
    test("free-letter", {"za", "zb"}, "abz");
    test("dup-edge", {"ca", "cb", "da", "db"}, "abcd");
    test("cycle", {"a", "b", "a"}, "");
    test("prefix-last", {"xyz", "xy"}, "");
    test("single", {"q"}, "q");
}
