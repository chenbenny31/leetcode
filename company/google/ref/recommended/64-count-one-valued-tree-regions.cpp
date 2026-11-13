// Count 1-Valued Tree Regions (Google, interview report)
// (1) binary tree in a vector pool, values 0 / 1; cnt maximal groups of connected 1-nodes
// (2) size of the largest region
//
// clarify:
// tree format -> pool, idx = id, child -1 if absent, root -1 = empty
// empty tree -> 0
// two 1-children of a 0-node -> two regions
// n and depth -> recursion; explicit stack as follow-up

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

// regions that start in the subtree at id; parentOne = id's parent has value 1
int dfs(const vector<Node>& tree, int id, bool parentOne) {
    if (id == -1) { return 0; }
    const Node& node = tree[id];
    bool one = node.val == 1;
    int count = (one && !parentOne) ? 1 : 0;
    return count + dfs(tree, node.left, one) + dfs(tree, node.right, one);
}

// (1) cnt regions
// core idea: each region has exactly one top = a 1-node that is root or has a 0 parent
// T: O(n), S: O(h)
int countRegions(const vector<Node>& tree, int root) {
    return dfs(tree, root, false);
}

// size of the 1-run hanging from id (0 for a 0-node or absent); updates best
int dfsSize(const vector<Node>& tree, int id, int& best) {
    if (id == -1) { return 0; }
    // visit both children even under a 0, so the regions below it are measured
    int down = dfsSize(tree, tree[id].left, best) + dfsSize(tree, tree[id].right, best);
    if (tree[id].val == 0) { return 0; }
    best = max(best, 1 + down);
    return 1 + down;
}

// (2) largest region
// core idea: post-order; a 1-node's run = 1 + children's runs; top holds the whole region
// T: O(n), S: O(h)
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
    test("empty", {}, -1, 0);

    testLargest("largest-split", {{1, 1, 2}, {0, 3}, {1}, {1}}, 0, 2);
    testLargest("largest-below-zero", {{0, 1, 2}, {1, 3, 4}, {1}, {1}, {1}}, 0, 3);
    testLargest("largest-all-zero", {{0, 1}, {0}}, 0, 0);
    testLargest("largest-empty", {}, -1, 0);
}

// follow-up:
// very deep tree -> same cnt w/ an explicit stack of (id, parentOne) pairs
// general graph / grid of 0-1 cells -> BFS / DFS from every unvisited 1, or union-find
// longest path of equal values (LC 687) -> dfs returns longest equal arm; best = L + R
