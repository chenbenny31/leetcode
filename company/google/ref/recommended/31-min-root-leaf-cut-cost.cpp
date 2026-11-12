// Min Cost to Cut the Root From All Leaves (Google, interview report). A binary tree,
// node 0 is the root; each child edge has a weight. Cut edges so that no leaf stays
// connected to the root; return the minimum total weight of the cut edges. A tree that is
// only the root has nothing to cut -> 0. Weights are non-negative.
//
// Clarify:
// - tree format -> vector pool, node 0 is the root; per node: left / right child id
//   (-1 if none) and the weight of the edge to that child
// - leaf = node with no children; a root with one child is not a leaf; the root alone
//   has nothing to cut -> 0
// - weights >= 0 and the total can pass 2^31 -> long long
// - n up to ~1e5; a skewed tree recurses n deep -> iterative follow-up
// - return only the min total weight (the edges themselves are a follow-up)
// Idea: cost(v) = cheapest way to cut v off from every leaf below it. Per child, either
//   cut the edge (w) or let the child cut itself off (cost(child)); different children
//   share no edges, so cost(v) = sum of the mins, one post-order pass. A leaf can never
//   be cut off from itself: INF, which forces a cut on the edge above it or higher.
// Pitfalls:
// - a leaf returns INF, not 0 (0 would mean "already cut off" and nothing gets cut)
// - handle the root-only tree before dfs, which would return INF for it
// - INF must exceed any total; min(w, INF) = w, so INF itself never enters a sum; cast
//   the int weight to long long inside min
// - skip a missing child (-1) instead of reading tree[-1]
// Tests to write: root only -> 0; two cheap cuts below beat one above; one cheap edge
//   above an expensive pair; a chain of one-child nodes; for the edges, a tie.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::queue;

struct Node {
    // child index, -1 if none
    int left;
    int leftWeight;
    int right;
    int rightWeight;
};

// a leaf cannot be separated from itself, so its cost is "infinite"
const long long INF = LLONG_MAX / 4;

// cost to separate node from every leaf below it: per child, either cut the edge to it
// or pay the child's own cost.
long long dfs(const vector<Node>& tree, int node) {
    const Node& cur = tree[node];
    if (cur.left == -1 && cur.right == -1) { return INF; }
    long long cost = 0;
    if (cur.left != -1) {
        cost += min((long long)cur.leftWeight, dfs(tree, cur.left));
    }
    if (cur.right != -1) {
        cost += min((long long)cur.rightWeight, dfs(tree, cur.right));
    }
    return cost;
}

// tree DP, cost(node) = sum over children of min(edge weight, cost(child)).
// T: O(n), S: O(h) recursion.
long long minCutCost(const vector<Node>& tree) {
    const Node& root = tree[0];
    if (root.left == -1 && root.right == -1) { return 0; }
    return dfs(tree, 0);
}

// follow-up "return the cut edges", also "very deep tree": the same DP with no
// recursion. BFS order puts parents before children, so a reverse pass fills cost[]
// bottom-up; a second BFS from the root cuts the edge to a child when its weight <=
// cost(child) (ties cut, fewer edges), else walks into the child. Returns the child id
// below each cut edge, in BFS order. T: O(n), S: O(n).
// Pitfall: the second pass must not enter a child whose edge was cut.
vector<int> minCutEdges(const vector<Node>& tree) {
    // node ids reachable from the root, parents before children
    vector<int> order = {0};
    for (int i = 0; i < (int)order.size(); i++) {
        const Node& cur = tree[order[i]];
        if (cur.left != -1) { order.push_back(cur.left); }
        if (cur.right != -1) { order.push_back(cur.right); }
    }
    // cost[id] = min weight to cut id off from every leaf below it, INF for a leaf
    vector<long long> cost(tree.size(), 0);
    for (int i = (int)order.size() - 1; i >= 0; i--) {
        int id = order[i];
        const Node& cur = tree[id];
        if (cur.left == -1 && cur.right == -1) {
            cost[id] = INF;
            continue;
        }
        if (cur.left != -1) {
            cost[id] += min((long long)cur.leftWeight, cost[cur.left]);
        }
        if (cur.right != -1) {
            cost[id] += min((long long)cur.rightWeight, cost[cur.right]);
        }
    }
    vector<int> cuts;
    // nodes still connected to the root
    queue<int> q;
    q.push(0);
    while (!q.empty()) {
        int id = q.front();
        q.pop();
        const Node& cur = tree[id];
        if (cur.left != -1 && cur.leftWeight <= cost[cur.left]) {
            cuts.push_back(cur.left);
        } else if (cur.left != -1) {
            q.push(cur.left);
        }
        if (cur.right != -1 && cur.rightWeight <= cost[cur.right]) {
            cuts.push_back(cur.right);
        } else if (cur.right != -1) {
            q.push(cur.right);
        }
    }
    return cuts;
}

void test(const string& label, const vector<Node>& tree, long long want) {
    cout << label << (minCutCost(tree) == want ? " PASS\n" : " FAIL\n");
}

void testCuts(const string& label, const vector<Node>& tree, const vector<int>& want) {
    cout << label << (minCutEdges(tree) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("root-only", {{-1, 0, -1, 0}}, 0);
    test("cut-below",
         {{1, 10, 2, 4}, {3, 2, 4, 3}, {-1, 0, -1, 0}, {-1, 0, -1, 0}, {-1, 0, -1, 0}},
         9);
    test("cut-above",
         {{1, 1, -1, 0}, {2, 5, 3, 5}, {-1, 0, -1, 0}, {-1, 0, -1, 0}}, 1);
    test("chain", {{1, 3, -1, 0}, {-1, 0, 2, 2}, {3, 7, -1, 0}, {-1, 0, -1, 0}}, 2);

    testCuts("cuts-root-only", {{-1, 0, -1, 0}}, {});
    testCuts("cuts-below",
             {{1, 10, 2, 4}, {3, 2, 4, 3}, {-1, 0, -1, 0}, {-1, 0, -1, 0},
              {-1, 0, -1, 0}},
             {2, 3, 4});
    testCuts("cuts-above",
             {{1, 1, -1, 0}, {2, 5, 3, 5}, {-1, 0, -1, 0}, {-1, 0, -1, 0}}, {1});
    testCuts("cuts-chain",
             {{1, 3, -1, 0}, {-1, 0, 2, 2}, {3, 7, -1, 0}, {-1, 0, -1, 0}}, {2});
    testCuts("cuts-tie",
             {{1, 5, -1, 0}, {2, 2, 3, 3}, {-1, 0, -1, 0}, {-1, 0, -1, 0}}, {1});
}

// follow-up: n-ary tree -> children as (child, weight) pairs, the same sum:
//     if (tree[node].children.empty()) { return INF; }
//     for (auto [child, w] : tree[node].children) {
//         cost += min((long long)w, dfs(tree, child));
//     }
// follow-up: return the cut edges -> minCutEdges above
// follow-up: only some leaves must be cut off -> a leaf that may stay costs 0:
//     if (cur.left == -1 && cur.right == -1) { return mustCut[node] ? INF : 0; }
// follow-up: very deep tree -> minCutEdges above fills cost[] with no recursion
// follow-up: general graph -> min s-t cut (max flow) with all leaves joined to a sink
