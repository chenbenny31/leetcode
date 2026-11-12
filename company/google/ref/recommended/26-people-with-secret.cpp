// LC 2092 Find All People With Secret (Hard). Pattern: union-find, reset per time group.
// n people; meetings[i] = [x, y, time]. Person 0 and firstPerson know the secret at
// time 0. In a meeting a knower tells the other; within one timestamp sharing is instant
// and transitive, but it never flows back to an earlier time. Return all knowers,
// ascending.
//
// Clarify:
// - within one time, sharing is instant and chains (a-b and b-c at time 5 tell c)
// - never backwards: a link made at time 5 cannot carry what is learned at time 6
// - a person may be in several meetings at one time; input not sorted by time
// - n, M up to 1e5 -> sort + union-find (or BFS per time group), not a scan per time
// - return every knower in ascending order, 0 and firstPerson included
// Idea: process time groups in increasing order. Union every pair of the group; those
//   now linked to 0 know. Everyone else in the group gets their links undone (parent
//   reset to self), so a group-only link cannot pass the secret on at a later time.
// Pitfalls:
// - union the whole group before any reset (the order of meetings inside a group does
//   not matter)
// - reset only the group's people not linked to 0; their sets hold only group people,
//   since every non-knower is a singleton between groups
// - compare roots (findRoot(parent, x) != findRoot(parent, 0)), not parent[x] != 0
// - sort by time only (a[2] < b[2]); the default sort orders by x first
// - unite 0 and firstPerson before the first meeting
// Tests to write: a chain inside one time; a later hop from someone who learned
//   earlier; no flow back in time; a group with a knowing and a non-knowing pair; no
//   meetings.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;
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

// follow-up: BFS per time group instead. Build the group's graph, start from members
// who already know, and mark everyone reached. Pitfall: the graph holds only this
// group's meetings, so build a fresh one per time. T: O(M log M + n), S: O(M + n).
vector<int> findAllPeopleBfs(int n, vector<vector<int>> meetings, int firstPerson) {
    sort(meetings.begin(), meetings.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[2] < b[2]; });
    // knows[p] = p knows the secret by now
    vector<bool> knows(n, false);
    knows[0] = true;
    knows[firstPerson] = true;
    int m = meetings.size();
    int start = 0;
    while (start < m) {
        // graph[p] = people p meets at this time
        unordered_map<int, vector<int>> graph;
        int end = start;
        while (end < m && meetings[end][2] == meetings[start][2]) {
            graph[meetings[end][0]].push_back(meetings[end][1]);
            graph[meetings[end][1]].push_back(meetings[end][0]);
            end++;
        }
        queue<int> q;
        for (const auto& [person, met] : graph) {
            if (knows[person]) { q.push(person); }
        }
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (int nxt : graph[cur]) {
                if (knows[nxt]) { continue; }
                knows[nxt] = true;
                q.push(nxt);
            }
        }
        start = end;
    }
    vector<int> result;
    for (int p = 0; p < n; p++) {
        if (knows[p]) { result.push_back(p); }
    }
    return result;
}

void test(const string& label, int n, const vector<vector<int>>& meetings,
          int firstPerson, const vector<int>& want) {
    bool ok = findAllPeople(n, meetings, firstPerson) == want;
    ok = ok && findAllPeopleBfs(n, meetings, firstPerson) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("same-time-chain", 4, {{2, 3, 5}, {1, 2, 5}}, 1, {0, 1, 2, 3});
    test("later-hop", 4, {{2, 3, 2}, {1, 2, 1}}, 1, {0, 1, 2, 3});
    test("no-back-in-time", 4, {{2, 3, 1}, {1, 2, 2}}, 1, {0, 1, 2});
    test("no-meetings", 3, {}, 2, {0, 2});
    test("mixed-group", 6, {{1, 2, 1}, {3, 4, 1}, {4, 5, 2}}, 1, {0, 1, 2});
}

// follow-up: BFS instead -> findAllPeopleBfs above
// follow-up: union by size -> keeps finds near O(alpha(n)) together with path halving;
//   in unite, with size(n, 1):
//   if (size[rootA] > size[rootB]) { swap(rootA, rootB); }
//   parent[rootA] = rootB;
//   size[rootB] += size[rootA];
//   (and reset size[x] = 1 together with parent[x] = x)
// follow-up: when did each person learn it -> in findAllPeopleBfs keep
//   learnedAt(n, -1), set to 0 for 0 and firstPerson, and next to knows[nxt] = true:
//   learnedAt[nxt] = meetings[start][2];
