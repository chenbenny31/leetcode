// ByteDance onsite (FT): vertices 0..numNodes-1; edge [u, v] means u comes before v.
// Return the lexicographically smallest topological order, or an empty array if there
// is a cycle (self-loops included). Duplicate edges are allowed.

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

// Kahn's algorithm with a min-heap instead of a FIFO queue: the smallest ready vertex
// always goes next. T: O((V + E) log V), S: O(V + E).
vector<int> topologicalOrder(int numNodes, const vector<vector<int>>& edges) {
    // graph[u] = vertices that must come after u
    vector<vector<int>> graph(numNodes);
    // indeg[v] = number of unprocessed edges into v
    vector<int> indeg(numNodes, 0);
    // duplicate edges count twice and are removed twice: no special case needed
    for (const vector<int>& e : edges) {
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
    test("smallest-first", 4, {{3, 0}, {1, 0}}, {1, 2, 3, 0});
    test("duplicate-edge", 2, {{1, 0}, {1, 0}}, {1, 0});
    test("self-loop", 2, {{1, 1}}, {});
    test("cycle", 3, {{0, 1}, {1, 0}}, {});
    test("empty-graph", 0, {}, {});
}

// follow-up: why not a FIFO queue -> edges 2->0, 2->1: FIFO gives 2,3,0,1 (3 is ready
//   early) instead of 2,0,1,3
// follow-up: is the order unique (LC 444) -> run Kahn; unique iff ready.size() == 1 at
//   every step and all V vertices get output
// follow-up: print the cycle -> DFS colors 0/1/2 + parent[]; back edge u -> v with v
//   color 1: walk parent from u up to v
// follow-up: min semesters / parallel layers (LC 1136) -> Kahn by BFS layers; answer =
//   number of layers, O(V + E)
