// alphabet? a-z
// order? lexi-min
// invalid input? return ""
// dup relation? no
// comp non-adj words? no
// empty or single-len word? yes

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

// build letter edges from adj words, then Kahn's toposort with min-heap
// T: O(L+26log26) for L total chars, S: O(26*26)
string alienOrder(const vector<string>& words) {
    vector<bool> present(26, false);
    // graph[a][b] = true, letter a must come before letter b
    vector<vector<bool>> graph(26, vector<bool>(26, false));
    vector<int> indeg(26, 0);
    for (const string& w : words) {
        for (char c : w) {
            present[c - 'a'] = true;
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

        if (j == len) { // one is prefix of the other
            if (a.size() > b.size()) {
                return "";
            }
            continue;
        }

        int u = a[j] - 'a';
        int v = b[j] - 'a';
        if (!graph[u][v]) {
            graph[u][v] = true;
            indeg[v]++;
        }
    }

    priority_queue<int, vector<int>, greater<int>> ready; // letter can be placed
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

    if ((int)order.size() < total) { return ""; } // remain letters on cycle
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
