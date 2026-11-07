// Page reachability: records[i] = [url, referrer]; a non-empty referrer means a visitor
// moved referrer -> url (one direction only). Is query[1] reachable from query[0] in
// zero or more moves?

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;
using std::unordered_set;

// BFS over referrer -> url edges; seen stops cycles. T: O(V+E), S: O(V+E).
bool canReachPage(const vector<vector<string>>& records, const vector<string>& query) {
    // graph[page] = pages a visitor reached directly from page
    unordered_map<string, vector<string>> graph;
    for (const auto& r : records) {
        if (r[1].empty()) { continue; }
        graph[r[1]].push_back(r[0]);
    }
    string start = query[0];
    string target = query[1];
    unordered_set<string> seen{start};
    queue<string> q;
    q.push(start);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        if (cur == target) { return true; }
        auto it = graph.find(cur);
        if (it == graph.end()) { continue; }
        for (const string& nxt : it->second) {
            if (seen.insert(nxt).second) { q.push(nxt); }
        }
    }
    return false;
}

void test(const string& label, const vector<vector<string>>& records,
          const vector<string>& query, bool want) {
    cout << label << (canReachPage(records, query) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<vector<string>> site = {{"/home", ""}, {"/shop", "/home"}, {"/cart", "/shop"}};
    test("chain", site, {"/home", "/cart"}, true);
    test("one-way", site, {"/cart", "/home"}, false);
    test("self", site, {"/cart", "/cart"}, true);
    test("cycle", {{"/a", "/b"}, {"/b", "/a"}}, {"/a", "/c"}, false);
}
