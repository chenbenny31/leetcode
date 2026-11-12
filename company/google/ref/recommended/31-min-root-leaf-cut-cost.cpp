// Min Cost to Cut the Root From All Leaves (Google, interview report). A binary tree,
// node 0 is the root; each child edge has a weight. Cut edges so that no leaf stays
// connected to the root; return the minimum total weight of the cut edges. A tree that is
// only the root has nothing to cut -> 0. Weights are non-negative.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::min;

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

void test(const string& label, const vector<Node>& tree, long long want) {
    cout << label << (minCutCost(tree) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("root-only", {{-1, 0, -1, 0}}, 0);
    test("cut-below",
         {{1, 10, 2, 4}, {3, 2, 4, 3}, {-1, 0, -1, 0}, {-1, 0, -1, 0}, {-1, 0, -1, 0}},
         9);
    test("cut-above",
         {{1, 1, -1, 0}, {2, 5, 3, 5}, {-1, 0, -1, 0}, {-1, 0, -1, 0}}, 1);
    test("chain", {{1, 3, -1, 0}, {-1, 0, 2, 2}, {3, 7, -1, 0}, {-1, 0, -1, 0}}, 2);
}

// follow-up: n-ary tree -> same sum over a children list
// follow-up: return the cut edges -> second pass: cut the edge when
//   weight <= cost(child), else recurse into the child
// follow-up: only some leaves must be cut off -> cost(other leaf) = 0, cost(target) = INF
// follow-up: very deep tree -> iterative post-order with an explicit stack
// follow-up: general graph -> min s-t cut (max flow) with all leaves joined to a sink
