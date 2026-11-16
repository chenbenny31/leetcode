// Players With a Known Rank (Google, interview report)
// n players 0 .. n - 1; results[i] = {winner, loser}; the better player always wins
// (1) how many players have their rank fixed by the results
// (2) rank of each such player (1 = best), -1 for the others
// reported: x3 (phone, L4 onsite, L5 onsite)
//
// clarify:
// results consistent -> yes, no cycle (a beats b beats a)
// transitive -> yes: a beat b, b beat c => a is above c
// same result twice -> harmless
// not everyone played -> fine, their rank is just unknown
// (1) -> entries of (2) that are not -1

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// num of nodes reachable from src, src not counted
int countReachable(const vector<vector<int>>& graph, int src) {
    int n = graph.size();
    vector<bool> seen(n, false);
    queue<int> q;
    seen[src] = true;
    q.push(src);
    int count = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (int nxt : graph[cur]) {
            if (seen[nxt]) { continue; }
            seen[nxt] = true;
            count++;
            q.push(nxt);
        }
    }
    return count;
}

// core idea: rank is fixed iff everyone else is known above or known below
// below = reachable in the win graph, above = reachable in the reversed graph
// T: O(V * (V + E)), S: O(V + E)
vector<int> knownRanks(int n, const vector<vector<int>>& results) {
    // graph[a] = players a beat; reversed[b] = players who beat b
    vector<vector<int>> graph(n);
    vector<vector<int>> reversed(n);
    for (const auto& result : results) {
        graph[result[0]].push_back(result[1]);
        reversed[result[1]].push_back(result[0]);
    }
    vector<int> ranks(n, -1);
    for (int i = 0; i < n; i++) {
        int below = countReachable(graph, i);
        int above = countReachable(reversed, i);
        if (below + above == n - 1) { ranks[i] = above + 1; }
    }
    return ranks;
}

void test(const string& label, int n, const vector<vector<int>>& results,
          const vector<int>& want) {
    cout << label << (knownRanks(n, results) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", 3, {{0, 1}, {1, 2}}, {1, 2, 3});
    test("diamond-middle-unknown", 4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}}, {1, -1, -1, 4});
    test("no-results", 2, {}, {-1, -1});
    test("dup-result", 2, {{0, 1}, {0, 1}}, {1, 2});
    test("one-never-played", 3, {{0, 1}}, {-1, -1, -1});
}

// follow-up:
// faster closure -> bitset per player in topological order, O(V * E / 64)
// Floyd-Warshall closure -> O(V^3), simplest to write when V is small
// is the whole ranking fixed -> Kahn: the queue never holds 2 players (LC 444 idea)
// results may contradict -> cycle check first (Kahn leaves players unpopped)
// "is a above b?" queries -> closure matrix once, O(1) each (LC 1462)
// new results arrive -> closure update: every ancestor of a reaches every descendant of b
