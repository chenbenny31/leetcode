// Count 1-Valued Tree Regions (Google, interview report). Binary tree in a vector pool
// (index = node id, children -1 if absent), every value 0 or 1, root id -1 = empty tree.
// A region is a maximal set of 1-valued nodes connected by parent-child edges. Return the
// number of regions.
//
// Clarify:
// - tree format -> vector pool, children -1 if absent; root -1 = empty tree -> 0
// - values only 0 / 1
// - connected = parent-child edges only: two 1-children of a 0-node are two regions
// - n up to ~1e5; a skewed tree recurses n deep -> iterative follow-up
// - return the number of regions (the largest region is a follow-up)
// Idea: every region has exactly one topmost node: a 1-node that is the root or has a 0
//   parent. One pre-order pass that passes "my parent is 1" down counts those tops in
//   O(n); a tree has no cycles, so no visited set.
// Pitfalls:
// - the root has no parent: start with parentOne = false, or a 1-root is never counted
// - pass the node's own value down to its children, not the value it received
// - check id == -1 before reading tree[id] (missing child and the empty tree)
// - two 1-children of a 1-node join its region: count tops, not 1-nodes or 1-children
// Tests to write: all ones -> 1; a 0 in the middle splits -> 2; a 0 root with two
//   1-children -> 2; all zeros -> 0; empty -> 0.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

struct Node {
    // 0 or 1
    int val;
    // child ids, -1 if absent
    int left = -1;
    int right = -1;
};

// regions starting in the subtree at id; parentOne = id has a parent with value 1.
// A 1-node starts a new region iff it is the root or its parent is 0.
int dfs(const vector<Node>& tree, int id, bool parentOne) {
    if (id == -1) { return 0; }
    const Node& node = tree[id];
    bool one = node.val == 1;
    int count = (one && !parentOne) ? 1 : 0;
    return count + dfs(tree, node.left, one) + dfs(tree, node.right, one);
}

// pre-order DFS counting region tops. T: O(n), S: O(h) recursion.
int countRegions(const vector<Node>& tree, int root) {
    return dfs(tree, root, false);
}

// size of the 1-run hanging from id (id plus the 1-nodes joined to it below), 0 if id is
// a 0-node or absent; updates best at every 1-node
int dfsSize(const vector<Node>& tree, int id, int& best) {
    if (id == -1) { return 0; }
    // visit both children even under a 0, so the regions below it are measured
    int down = dfsSize(tree, tree[id].left, best) + dfsSize(tree, tree[id].right, best);
    if (tree[id].val == 0) { return 0; }
    best = max(best, 1 + down);
    return 1 + down;
}

// follow-up "largest region": post-order; a 1-node's run is 1 + its children's runs, so
// a region's top node holds the whole region. T: O(n), S: O(h) recursion.
// Pitfall: a 0-node returns 0 but must still recurse, or regions under it are missed.
int largestRegion(const vector<Node>& tree, int root) {
    int best = 0;
    dfsSize(tree, root, best);
    return best;
}

void test(const string& label, const vector<Node>& tree, int root, int want) {
    cout << label << (countRegions(tree, root) == want ? " PASS\n" : " FAIL\n");
}

void testLargest(const string& label, const vector<Node>& tree, int root, int want) {
    cout << label << (largestRegion(tree, root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("all-ones", {{1, 1, 2}, {1}, {1}}, 0, 1);
    test("split-by-zero", {{1, 1, 2}, {0, 3}, {1}, {1}}, 0, 2);
    test("zero-root", {{0, 1, 2}, {1}, {1}}, 0, 2);
    test("all-zero", {{0, 1}, {0}}, 0, 0);
    test("empty", {}, -1, 0);

    testLargest("largest-split", {{1, 1, 2}, {0, 3}, {1}, {1}}, 0, 2);
    testLargest("largest-below-zero", {{0, 1, 2}, {1, 3, 4}, {1}, {1}, {1}}, 0, 3);
    testLargest("largest-all-zero", {{0, 1}, {0}}, 0, 0);
    testLargest("largest-empty", {}, -1, 0);
}

// follow-up: largest region size -> largestRegion above
// follow-up: very deep tree -> the same count with an explicit stack of (id, parentOne):
//     vector<pair<int, bool>> st = {{root, false}};
//     while (!st.empty()) { auto [id, parentOne] = st.back(); st.pop_back();
//         if (id == -1) { continue; }
//         bool one = tree[id].val == 1;
//         if (one && !parentOne) { count++; }
//         st.push_back({tree[id].left, one}); st.push_back({tree[id].right, one}); }
// follow-up: general graph / grid of 0-1 cells -> same count via BFS / DFS from every
//   unvisited 1 (select/08-graphs/01-number-of-islands), or union-find over 1-1 edges
