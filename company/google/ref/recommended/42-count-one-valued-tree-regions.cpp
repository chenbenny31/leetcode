// Count 1-Valued Tree Regions (Google, interview report). Binary tree in a vector pool
// (index = node id, children -1 if absent), every value 0 or 1, root id -1 = empty tree.
// A region is a maximal set of 1-valued nodes connected by parent-child edges. Return the
// number of regions.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

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

void test(const string& label, const vector<Node>& tree, int root, int want) {
    cout << label << (countRegions(tree, root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("all-ones", {{1, 1, 2}, {1}, {1}}, 0, 1);
    test("split-by-zero", {{1, 1, 2}, {0, 3}, {1}, {1}}, 0, 2);
    test("zero-root", {{0, 1, 2}, {1}, {1}}, 0, 2);
    test("all-zero", {{0, 1}, {0}}, 0, 0);
    test("empty", {}, -1, 0);
}

// follow-up: largest region size -> dfs returns the size of the 1-run hanging from id
//   through its parent link (0 if id is 0) and updates best at every 1-node:
//     // visit both children first so 1-runs below a 0 are still measured
//     int down = dfs(left) + dfs(right);
//     if (val == 0) { return 0; }
//     best = max(best, 1 + down);
//     return 1 + down;
// follow-up: general graph / grid of 0-1 cells -> same count via BFS / DFS from every
//   unvisited 1 (number of islands), or union-find over 1-1 edges
