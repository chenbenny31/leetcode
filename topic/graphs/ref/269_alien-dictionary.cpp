// LC 269 Alien Dictionary: words are sorted by an unknown alien alphabet. Return one
// valid order of the letters that appear in them, or "" if no order is consistent.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_set;
using std::min;

// Only the first differing letters of adjacent words give an edge; then Kahn's BFS.
// T: O(C + V + E), S: O(V + E); C = total characters, V <= 26.
string alienOrder(const vector<string>& words) {
    vector<unordered_set<int>> graph(26);   // graph[a] = letters that must come after a
    vector<int> indeg(26, 0);               // indeg[c] = distinct letters before c
    vector<bool> present(26, false);        // present[c] = c appears in some word
    for (const string& w : words) {
        for (char c : w) { present[c - 'a'] = true; }
    }
    int n = words.size();
    for (int i = 0; i + 1 < n; i++) {
        const string& a = words[i];
        const string& b = words[i + 1];
        int len = min(a.size(), b.size());
        int j = 0;
        while (j < len && a[j] == b[j]) { j++; }
        if (j == len) {
            // b is a proper prefix of a yet comes after it: impossible
            if (a.size() > b.size()) { return ""; }
            continue;
        }
        // later letters say nothing: the pair is already ordered at position j
        int u = a[j] - 'a', v = b[j] - 'a';
        if (graph[u].insert(v).second) { indeg[v]++; }
    }
    queue<int> q;   // present letters with no unplaced predecessor
    int total = 0;
    for (int c = 0; c < 26; c++) {
        if (!present[c]) { continue; }
        total++;
        if (indeg[c] == 0) { q.push(c); }
    }
    string order;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        order += (char)('a' + cur);
        for (int nxt : graph[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    // letters left out sit on a cycle
    return (int)order.size() == total ? order : "";
}

void test(const string& label, const vector<string>& words, const string& want) {
    cout << label << (alienOrder(words) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {"ba", "bc", "ac", "cab"}, "bac");
    test("cycle", {"a", "b", "a"}, "");
    test("bad-prefix", {"abc", "ab"}, "");
    test("duplicate-edge", {"aa", "ab", "ba"}, "ab");
}
