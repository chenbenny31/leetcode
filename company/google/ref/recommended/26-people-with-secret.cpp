// LC 2092 Find All People With Secret (Hard). Pattern: union-find, reset per time group.
// n people; meetings[i] = [x, y, time]. Person 0 and firstPerson know the secret at
// time 0. In a meeting a knower tells the other; within one timestamp sharing is instant
// and transitive, but it never flows back to an earlier time. Return all knowers,
// ascending.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// returns the root of node, halving the path on the way
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

// Sort by time; per time group union every pair, then whoever is not linked to 0 forgets
// the group's links (parent reset to self). T: O(M log M + (M + n) log n), S: O(n).
vector<int> findAllPeople(int n, vector<vector<int>> meetings, int firstPerson) {
    sort(meetings.begin(), meetings.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[2] < b[2]; });
    // parent[p] = parent of person p; the set holding 0 = everyone who knows
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
        // links not reaching 0 must not carry the secret at a later time
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
    test("no-meetings", 3, {}, 2, {0, 2});
}

// follow-up: BFS instead -> per time group build graph[person] = people met at that time,
//   BFS from the group members who already know, mark everyone reached; O(M log M + n)
// follow-up: union by size -> keeps finds near O(alpha(n)) together with path halving
