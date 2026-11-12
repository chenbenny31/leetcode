// LC 207 Course Schedule (Medium). Pattern: topological sort (Kahn's BFS).
// numCourses courses 0..n-1, prerequisites [a, b] = take b before a. Can all courses be
// finished? LC 210: return one valid order, or {} if impossible.
//
// Clarify:
// - [a, b] means b before a (edge b -> a) -> confirm the direction out loud
// - duplicate pairs? -> fine: indegree counts each copy and each copy is removed once;
//   a self-loop [a, a] is a cycle
// - several valid orders -> any one (the lexicographically smallest is a follow-up)
// - courses with no prerequisites still appear in the order
// - cycle -> false / {}
// Idea: Kahn: indeg[c] = prerequisites of c not taken yet. Start from every course with
//   indegree 0; take one, decrement the courses it unlocks, push those that reach 0. A
//   cycle never reaches 0, so fewer than n courses get taken.
// Pitfalls:
// - edge direction: graph[prereq].push_back(course), indeg[course]++
// - detect a cycle by order.size() < n at the end
// - decrement first, then test == 0 (no if (--indeg[nxt] == 0))
// - the DFS version needs 3 states (new / on path / done): with 2 states a diamond
//   (two paths into one course) looks like a cycle
// Tests to write: diamond; 2-cycle; cycle not reachable from course 0; self-loop; no
//   prerequisites -> 0..n-1.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::priority_queue;
using std::greater;
using std::unordered_map;

// Kahn's BFS: keep taking courses with indegree 0; a cycle leaves some never taken.
// T: O(V+E), S: O(V+E).
bool canFinish(int n, const vector<vector<int>>& prerequisites) {
    // graph[prereq] = courses it unlocks (edge prereq -> course)
    vector<vector<int>> graph(n);
    // indeg[c] = prereqs of c not taken yet
    vector<int> indeg(n, 0);
    for (const vector<int>& req : prerequisites) {
        int course = req[0];
        int prereq = req[1];
        graph[prereq].push_back(course);
        indeg[course]++;
    }
    // courses whose prereqs are all taken
    queue<int> q;
    for (int c = 0; c < n; c++) {
        if (indeg[c] == 0) { q.push(c); }
    }
    int taken = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        taken++;
        for (int nxt : graph[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    // untaken courses sit on or behind a cycle
    return taken == n;
}

// LC 210: same Kahn's BFS, recording the pop order. T: O(V+E), S: O(V+E).
vector<int> findOrder(int n, const vector<vector<int>>& prerequisites) {
    // graph[prereq] = courses it unlocks (edge prereq -> course)
    vector<vector<int>> graph(n);
    // indeg[c] = prereqs of c not taken yet
    vector<int> indeg(n, 0);
    for (const vector<int>& req : prerequisites) {
        int course = req[0];
        int prereq = req[1];
        graph[prereq].push_back(course);
        indeg[course]++;
    }
    queue<int> q;
    for (int c = 0; c < n; c++) {
        if (indeg[c] == 0) { q.push(c); }
    }
    // courses in the order taken
    vector<int> order;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        order.push_back(cur);
        for (int nxt : graph[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    if ((int)order.size() < n) { return {}; }
    return order;
}

// 3-color DFS from cur; true if it reaches a course still on the path (a cycle)
bool dfs(int cur, const vector<vector<int>>& graph, vector<int>& state) {
    state[cur] = 1;
    for (int nxt : graph[cur]) {
        if (state[nxt] == 1) { return true; }
        if (state[nxt] == 0 && dfs(nxt, graph, state)) { return true; }
    }
    state[cur] = 2;
    return false;
}

// 3-color DFS from every unvisited course; reaching a course on the current path is a
// cycle. Reversed post-order would give a valid order. T: O(V+E), S: O(V+E).
bool canFinishDfs(int n, const vector<vector<int>>& prerequisites) {
    // graph[prereq] = courses it unlocks (edge prereq -> course)
    vector<vector<int>> graph(n);
    for (const vector<int>& req : prerequisites) {
        int course = req[0];
        int prereq = req[1];
        graph[prereq].push_back(course);
    }
    // state[c]: 0 new, 1 on the current DFS path, 2 done (no cycle reachable)
    vector<int> state(n, 0);
    for (int c = 0; c < n; c++) {
        if (state[c] == 0 && dfs(c, graph, state)) { return false; }
    }
    return true;
}

// Google: the lexicographically smallest valid order, {} if a cycle. Kahn's with a
// min-heap of ready courses: always take the smallest course available.
// T: O(V log V + E), S: O(V+E).
vector<int> smallestOrder(int n, const vector<vector<int>>& prerequisites) {
    // graph[prereq] = courses it unlocks (edge prereq -> course)
    vector<vector<int>> graph(n);
    // indeg[c] = prereqs of c not taken yet
    vector<int> indeg(n, 0);
    for (const vector<int>& req : prerequisites) {
        int course = req[0];
        int prereq = req[1];
        graph[prereq].push_back(course);
        indeg[course]++;
    }
    // courses whose prereqs are all taken, smallest on top
    priority_queue<int, vector<int>, greater<int>> ready;
    for (int c = 0; c < n; c++) {
        if (indeg[c] == 0) { ready.push(c); }
    }
    vector<int> order;
    while (!ready.empty()) {
        int cur = ready.top();
        ready.pop();
        order.push_back(cur);
        for (int nxt : graph[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) { ready.push(nxt); }
        }
    }
    if ((int)order.size() < n) { return {}; }
    return order;
}

// follow-up (LC 1136; also min finish time with unlimited parallel tasks): fewest
// semesters when any number of courses fit in one term; -1 on a cycle. Kahn level by
// level: one level = one semester, so the answer is the longest chain.
// T: O(V+E), S: O(V+E).
int minSemesters(int n, const vector<vector<int>>& prerequisites) {
    // graph[prereq] = courses it unlocks (edge prereq -> course)
    vector<vector<int>> graph(n);
    // indeg[c] = prereqs of c not taken yet
    vector<int> indeg(n, 0);
    for (const vector<int>& req : prerequisites) {
        graph[req[1]].push_back(req[0]);
        indeg[req[0]]++;
    }
    // courses that can be taken this semester
    queue<int> q;
    for (int c = 0; c < n; c++) {
        if (indeg[c] == 0) { q.push(c); }
    }
    int semesters = 0;
    int taken = 0;
    while (!q.empty()) {
        semesters++;
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++) {
            int cur = q.front();
            q.pop();
            taken++;
            for (int nxt : graph[cur]) {
                indeg[nxt]--;
                if (indeg[nxt] == 0) { q.push(nxt); }
            }
        }
    }
    return taken == n ? semesters : -1;
}

// follow-up (Google: merge ordered subsequences / master sequence, ×4): each list is a
// subsequence of one hidden sequence of distinct values; rebuild it. An edge between
// each adjacent pair, then Kahn; the answer is unique only if the queue never holds 2+
// values. Returns {} on a cycle or when the order is not unique.
// T: O(V+E) expected (hash maps), S: O(V+E).
vector<int> masterSequence(const vector<vector<int>>& seqs) {
    // graph[v] = values that come right after v in some list
    unordered_map<int, vector<int>> graph;
    // indeg[v] = edges into v not removed yet; every value is a key
    unordered_map<int, int> indeg;
    for (const vector<int>& seq : seqs) {
        for (int v : seq) { indeg.insert({v, 0}); }
        for (int i = 0; i + 1 < (int)seq.size(); i++) {
            graph[seq[i]].push_back(seq[i + 1]);
            indeg[seq[i + 1]]++;
        }
    }
    queue<int> q;
    for (const auto& [v, d] : indeg) {
        if (d == 0) { q.push(v); }
    }
    vector<int> order;
    while (!q.empty()) {
        // two values could come next: the lists do not fix the order
        if (q.size() > 1) { return {}; }
        int cur = q.front();
        q.pop();
        order.push_back(cur);
        for (int nxt : graph[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    if (order.size() < indeg.size()) { return {}; }
    return order;
}

void test(const string& label, int n, const vector<vector<int>>& prerequisites,
          const vector<int>& want) {
    bool ok = canFinish(n, prerequisites) == !want.empty();
    ok = ok && findOrder(n, prerequisites) == want;
    ok = ok && canFinishDfs(n, prerequisites) == !want.empty();
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testSmallest(const string& label, int n, const vector<vector<int>>& prerequisites,
                  const vector<int>& want) {
    bool ok = smallestOrder(n, prerequisites) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testSemesters(const string& label, int n, const vector<vector<int>>& prerequisites,
                   int want) {
    cout << label << (minSemesters(n, prerequisites) == want ? " PASS\n" : " FAIL\n");
}

void testMaster(const string& label, const vector<vector<int>>& seqs,
                const vector<int>& want) {
    cout << label << (masterSequence(seqs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, {0, 1, 2, 3});
    test("cycle", 2, {{1, 0}, {0, 1}}, {});
    test("partial-cycle", 3, {{1, 0}, {2, 1}, {1, 2}}, {});
    test("no-prereqs", 2, {}, {0, 1});
    test("self-loop", 2, {{1, 1}}, {});
    test("cycle-off-start", 3, {{2, 1}, {1, 2}}, {});
    testSmallest("smallest-unlock-first", 4, {{1, 0}}, {0, 1, 2, 3});
    testSmallest("smallest-reversed-chain", 3, {{0, 1}, {1, 2}}, {2, 1, 0});
    testSmallest("smallest-cycle", 2, {{1, 0}, {0, 1}}, {});
    testSemesters("terms-diamond", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, 3);
    testSemesters("terms-no-prereqs", 3, {}, 1);
    testSemesters("terms-cycle", 2, {{1, 0}, {0, 1}}, -1);
    testMaster("master-two-lists", {{1, 2, 3}, {1, 3, 4}}, {1, 2, 3, 4});
    testMaster("master-single-list", {{5, 3, 8}}, {5, 3, 8});
    testMaster("master-ambiguous", {{1, 2}, {1, 3}}, {});
    testMaster("master-cycle", {{1, 2}, {2, 1}}, {});
}

// follow-up: min semesters, no per-term limit (LC 1136) -> minSemesters above
// follow-up: merge ordered sequences / common parent sequence (J3) -> masterSequence
//   above; to return any merged order instead, drop the q.size() > 1 check
// follow-up: at most k courses per term (LC 1494) -> NP-hard in general; bitmask DP over
//   taken sets for n <= 15, not greedy
// follow-up: recipes from stock (LC 2115, J48) -> Kahn with supplies as the initial
//   sources
