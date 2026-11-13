// LC 2092 Find All People With Secret (Hard). Pattern: union-find, reset per time group.
// meetings[i] = {x, y, time}; 0 and firstPerson know at time 0; a knower tells the other
// same-time sharing is instant + transitive, never flows back in time; return knowers asc
//
// clarify:
// chain in one timestamp (a-b, b-c) -> c learns it
// one person in several meetings at one time -> ok
// input sorted by time? -> no, sort first
// n, M up to 1e5 -> need ~O(M log M)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// root of node, halving the path on the way
int findRoot(vector<int>& parent, int node) {
    while (parent[node] != node) {
        parent[node] = parent[parent[node]];
        node = parent[node];
    }
    return node;
}

void unite(vector<int>& parent, int a, int b) {
    int rootA = findRoot(parent, a);
    int rootB = findRoot(parent, b);
    parent[rootA] = rootB;
}

// core idea: sort by time; per time group union all pairs, then reset those not tied to 0
// reset = parent to self, so a group-only link cannot pass the secret later
// T: O(M log M + (M + n) log n), S: O(n)
vector<int> findAllPeople(int n, vector<vector<int>> meetings, int firstPerson) {
    sort(meetings.begin(), meetings.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[2] < b[2]; });
    // parent[p] = parent of person p; set holding 0 = everyone who knows
    vector<int> parent(n);
    for (int i = 0; i < n; i++) { parent[i] = i; }
    unite(parent, 0, firstPerson);

    int m = meetings.size();
    int start = 0;
    while (start < m) {
        int end = start;
        while (end < m && meetings[end][2] == meetings[start][2]) {
            unite(parent, meetings[end][0], meetings[end][1]);
            end++;
        }
        // links not reaching 0 must not carry the secret to a later time
        for (int i = start; i < end; i++) {
            int x = meetings[i][0];
            int y = meetings[i][1];
            if (findRoot(parent, x) != findRoot(parent, 0)) { parent[x] = x; }
            if (findRoot(parent, y) != findRoot(parent, 0)) { parent[y] = y; }
        }
        start = end;
    }

    vector<int> result;
    for (int p = 0; p < n; p++) {
        if (findRoot(parent, p) == findRoot(parent, 0)) { result.push_back(p); }
    }
    return result;
}

void test(const string& label, int n, const vector<vector<int>>& meetings,
          int firstPerson, const vector<int>& want) {
    bool ok = findAllPeople(n, meetings, firstPerson) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("same-time-chain", 4, {{2, 3, 5}, {1, 2, 5}}, 1, {0, 1, 2, 3});
    test("later-hop", 4, {{2, 3, 2}, {1, 2, 1}}, 1, {0, 1, 2, 3});
    test("no-back-in-time", 4, {{2, 3, 1}, {1, 2, 2}}, 1, {0, 1, 2});
    test("mixed-group", 6, {{1, 2, 1}, {3, 4, 1}, {4, 5, 2}}, 1, {0, 1, 2});
}

// follow-up:
// BFS instead of union-find -> per time group build its graph, BFS from knowers
// union by size -> near O(alpha(n)) finds; reset size[x] = 1 w/ parent[x] = x
// when did each learn it -> learnedAt[p] = group time when p newly links to 0
// why union whole group before any reset -> meeting order in a timestamp must not matter
