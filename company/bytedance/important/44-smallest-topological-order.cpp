// ByteDance onsite (FT): vertices 0..numNodes-1; each edge [u, v] means u must come
// before v. Return the lexicographically smallest topological order. If the graph has
// a cycle (including a self-loop), return an empty array - never a half-built prefix.
// An empty graph (numNodes = 0) also returns an empty array. Duplicate edges do not
// change the constraints.
//   topologicalOrder(numNodes, edges) -> vector<int>

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;

// Kahn's algorithm, but the ready set is a min-heap instead of a FIFO queue: at every
// step the smallest vertex with no remaining prerequisites goes next, which gives the
// lexicographically smallest order. A FIFO queue would output 2,3,0,1 for edges
// 2->0, 2->1 (3 is ready early) instead of 2,0,1,3.
// Duplicate edges add to indeg twice and are removed twice, so they need no special
// case; a self-loop keeps its vertex's indeg above 0 forever, so it shows as a cycle.
// T: O((V + E) log V), S: O(V + E).
vector<int> topologicalOrder(int numNodes, const vector<vector<int>>& edges) {
    // graph[u] = vertices that must come after u
    vector<vector<int>> graph(numNodes);
    // indeg[v] = number of unprocessed edges into v
    vector<int> indeg(numNodes, 0);
    for (const auto& e : edges) {
        graph[e[0]].push_back(e[1]);
        indeg[e[1]]++;
    }

    // ready = vertices whose prerequisites are all placed, smallest on top
    priority_queue<int, vector<int>, greater<int>> ready;
    for (int v = 0; v < numNodes; v++) {
        if (indeg[v] == 0) {
            ready.push(v);
        }
    }

    vector<int> order;
    while (!ready.empty()) {
        int cur = ready.top();
        ready.pop();
        order.push_back(cur);
        for (int nxt : graph[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) {
                ready.push(nxt);
            }
        }
    }

    // some vertex never became ready: it sits on a cycle
    if ((int)order.size() < numNodes) {
        return {};
    }
    return order;
}

void test(const string& label, int n, const vector<vector<int>>& edges,
          const vector<int>& want) {
    cout << label << (topologicalOrder(n, edges) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", 3, {{2, 1}, {1, 0}}, {2, 1, 0});
    test("smallest-first", 4, {{3, 0}, {1, 0}}, {1, 2, 3, 0});
    test("duplicate-edge", 2, {{1, 0}, {1, 0}}, {1, 0});
    test("self-loop", 2, {{1, 1}}, {});
    test("cycle", 3, {{0, 1}, {1, 0}}, {});
    test("empty-graph", 0, {}, {});
}
