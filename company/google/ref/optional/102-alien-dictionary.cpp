// 102 · Alien Dictionary — LC 269
// Mother problem: select/optional/08-graphs/08-alien-dictionary.cpp
//
// words are sorted by an unknown alien alphabet. Return any valid order of the letters
// that appear, or "" if none is consistent (cycle, or a word before its own prefix).
//
// Clarify
// - letters? → lowercase a–z; output only letters that appear
// - several valid orders? → return any one
// - invalid? → cycle, or a word followed by its own proper prefix → ""
// - duplicate constraints? → dedupe edges, or indegrees never reach 0
//
// Core idea
// - Each adjacent word pair gives one edge: first differing letter before → after.
// - graph[c] = unordered_set of letters after c; indeg[c] = distinct letters before c.
// - Kahn's BFS from present letters with indeg 0; if fewer letters come out than are
//   present, there is a cycle.
// - T O(C + V + E) with C total chars, V ≤ 26; S O(V + E).
//
// Tests
// - chain {"ba","bc","ac","cab"} → "bac"
// - duplicate edge {"aa","ab","ba"} → "ab"
// - cycle {"a","b","a"} → ""
// - bad prefix {"abc","ab"} → ""
//
// Follow-ups
// - lexicographically smallest order → min-heap instead of the queue
// - is the order unique? → queue size > 1 at any pop means not unique

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

// First differing letter of each adjacent pair gives an edge; then Kahn's BFS.
// T: O(C + V + E), S: O(V + E); C = total characters, V <= 26.
string alienOrder(const vector<string>& words) {
    // graph[c] = letters that must come after c (set dedupes edges)
    vector<unordered_set<int>> graph(26);
    // indeg[c] = distinct letters that must come before c
    vector<int> indeg(26, 0);
    // present[c] = c appears in some word
    vector<bool> present(26, false);
    for (const string& word : words) {
        for (char c : word) { present[c - 'a'] = true; }
    }
    for (int i = 0; i + 1 < (int)words.size(); i++) {
        const string& first = words[i];
        const string& second = words[i + 1];
        int len = min(first.size(), second.size());
        int pos = 0;
        while (pos < len && first[pos] == second[pos]) { pos++; }
        if (pos == len) {
            // second is a proper prefix of first yet comes after it: impossible
            if (first.size() > second.size()) { return ""; }
            continue;
        }
        int before = first[pos] - 'a';
        int after = second[pos] - 'a';
        // count each edge once, or indeg[after] never reaches 0
        if (graph[before].count(after)) { continue; }
        graph[before].insert(after);
        indeg[after]++;
    }
    // present letters with no unplaced predecessor
    queue<int> q;
    int letterCount = 0;
    for (int c = 0; c < 26; c++) {
        if (!present[c]) { continue; }
        letterCount++;
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
    return (int)order.size() == letterCount ? order : "";
}

void test(const string& label, const vector<string>& words, const string& want) {
    cout << label << (alienOrder(words) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {"ba", "bc", "ac", "cab"}, "bac");
    test("dup-edge", {"aa", "ab", "ba"}, "ab");
    test("cycle", {"a", "b", "a"}, "");
    test("bad-prefix", {"abc", "ab"}, "");
}
