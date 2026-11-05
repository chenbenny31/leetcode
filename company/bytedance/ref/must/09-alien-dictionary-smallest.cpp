// ByteDance (FastPrep, phone / onsite) / LC 269 variant: words (lowercase) are sorted in
// an unknown alphabet. Return the lexicographically smallest order of the letters that
// appear, consistent with words, or "" if none. Adjacent words: the first differing
// letter decides; if one is a prefix of the other, the shorter must come first.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::min;

// Edges from adjacent words, then Kahn's with a min-heap: smallest ready letter first.
// T: O(L + 26^2) for L total characters, S: O(26^2).
string alienOrder(const vector<string>& words) {
    // present[c] = letter c appears in some word
    vector<bool> present(26, false);
    // graph[a][b] = true when letter a must come before letter b (matrix dedupes edges)
    vector<vector<bool>> graph(26, vector<bool>(26, false));
    // indeg[b] = number of distinct letters that must come before b
    vector<int> indeg(26, 0);
    for (const string& w : words) {
        for (char ch : w) {
            present[ch - 'a'] = true;
        }
    }

    for (int i = 0; i + 1 < (int)words.size(); i++) {
        const string& a = words[i];
        const string& b = words[i + 1];
        int len = min(a.size(), b.size());
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
        // count each relation once, or indeg never reaches 0
        if (!graph[u][v]) {
            graph[u][v] = true;
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
            if (graph[cur][nxt]) {
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
}

// follow-up: any valid order (LC 269) -> plain queue instead of min-heap, O(L)
// follow-up: is the order unique? -> ready.size() > 1 at any pop means two letters are
//   unordered -> not unique
// follow-up: verify with known alphabet (LC 953) -> rank[c] = index in order; compare
//   adjacent words by rank, same prefix rule, O(L)
// follow-up: DFS instead of Kahn -> 3-color DFS, gray hit = cycle, reverse post-order
//   (gives a valid order, not the smallest one)
