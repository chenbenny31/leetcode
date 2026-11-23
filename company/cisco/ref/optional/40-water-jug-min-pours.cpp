// 40 · Water Jugs, Min Pours (Cisco OA, Medium) — BFS over jug states
// jug i has capacity cap[i] and holds start[i]; one pour moves water from jug i to jug j
// until i is empty or j is full; return min pours to reach target[] exactly, -1 if never
//
// clarify:
// fill from tap / empty into sink allowed? -> assume no, pours only (see follow-up)
// target = whole configuration or one volume in any jug? -> configuration (see follow-up)
// num of jugs, capacities -> small (3-4 jugs, cap <= ~100), so all states fit in a set
// start == target -> 0

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::set;
using std::min;

// core idea: state = volumes of all jugs; a pour is an edge of cost 1 -> BFS by layers
// T: O(S * n^2 * n log S), S: O(S * n) — S = num of reachable states, n = num of jugs
int minPours(const vector<int>& cap, const vector<int>& start,
             const vector<int>& target) {
    int n = cap.size();
    // states already queued
    set<vector<int>> seen;
    // states reachable with exactly `steps` pours
    queue<vector<int>> q;
    q.push(start);
    seen.insert(start);
    int steps = 0;
    while (!q.empty()) {
        int sz = q.size();
        for (int k = 0; k < sz; k++) {
            vector<int> cur = q.front();
            q.pop();
            if (cur == target) { return steps; }
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (i == j) { continue; }
                    // pour i -> j stops when i is empty or j is full
                    int moved = min(cur[i], cap[j] - cur[j]);
                    if (moved == 0) { continue; }
                    vector<int> nxt = cur;
                    nxt[i] -= moved;
                    nxt[j] += moved;
                    if (seen.count(nxt) == 0) {
                        seen.insert(nxt);
                        q.push(nxt);
                    }
                }
            }
        }
        steps++;
    }
    return -1;
}

void test(const string& label, const vector<int>& cap, const vector<int>& start,
          const vector<int>& target, int want) {
    cout << label << (minPours(cap, start, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("8-5-3", {8, 5, 3}, {8, 0, 0}, {4, 4, 0}, 7);
    test("one-pour", {8, 5, 3}, {8, 0, 0}, {3, 5, 0}, 1);
    test("same", {8, 5, 3}, {8, 0, 0}, {8, 0, 0}, 0);
    test("impossible", {4, 2}, {4, 0}, {3, 1}, -1);
    return 0;
}

// follow-up:
// 1. fill / empty allowed -> two more moves per jug: nxt[i] = cap[i], nxt[i] = 0
// 2. target = volume t in any jug -> goal check: some cur[i] == t
// 3. 2 jugs, only "is t reachable" (LC 365) -> t <= a + b and t % gcd(a, b) == 0, no BFS
// 4. print the pours -> map state -> (prev state, i, j), walk back from target
// 5. faster states -> encode volumes as one int (base cap + 1), unordered_set
