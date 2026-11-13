// LC 298 + LC 549 Binary Tree Longest Consecutive Sequence I / II. Length = node count.
// (1) LC 298: path goes parent -> child only, each value = previous + 1.
// (2) LC 549: path may go child -> parent -> child, values consecutive and either all
//     increasing or all decreasing along the path. Empty tree -> 0.
//
// Clarify:
// - (1) decreasing steps do not count; only +1 downward
// - (2) the path bends at most once (at its top node)
// Idea (1): dfs(node) = longest +1 run starting at node going down: 1 + dfs(child) when
//   child->val == node->val + 1, else 1; keep a global max.
// Idea (2): dfs(node) = (inc, dec): longest downward path from node where each next
//   value is +1 / -1. The path through node is inc + dec - 1 (one side counts up into
//   node, the other keeps going); keep a global max.
// Pitfalls:
// - recurse into every child even when it breaks the run (a new run starts below)
// - inc + dec - 1: node is counted in both
// - pool nodes in a reserved vector so pointers stay valid and nothing leaks
// Tests to write: +1 chain with a repeat; chain with down steps (1 vs 2 differ);
//   root between children 1 and 3; single node; empty tree.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::max;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// (1) longest +1 run going down from node; best = global max. T: O(n), S: O(h).
int dfsDown(TreeNode* node, int& best) {
    if (!node) { return 0; }
    int run = 1;
    TreeNode* children[2] = {node->left, node->right};
    for (TreeNode* child : children) {
        int childRun = dfsDown(child, best);
        if (child && child->val == node->val + 1) { run = max(run, childRun + 1); }
    }
    best = max(best, run);
    return run;
}

int longestConsecutive(TreeNode* root) {
    int best = 0;
    dfsDown(root, best);
    return best;
}

// (2) returns (inc, dec) = longest downward +1 / -1 paths from node; the path bending
// at node has inc + dec - 1 nodes. T: O(n), S: O(h).
pair<int, int> dfsUpDown(TreeNode* node, int& best) {
    int inc = 1;
    int dec = 1;
    TreeNode* children[2] = {node->left, node->right};
    for (TreeNode* child : children) {
        if (!child) { continue; }
        auto [childInc, childDec] = dfsUpDown(child, best);
        if (child->val == node->val + 1) { inc = max(inc, childInc + 1); }
        if (child->val == node->val - 1) { dec = max(dec, childDec + 1); }
    }
    best = max(best, inc + dec - 1);
    return {inc, dec};
}

int longestConsecutive2(TreeNode* root) {
    if (!root) { return 0; }
    int best = 0;
    dfsUpDown(root, best);
    return best;
}

const int NUL = -1;

// LC level order (NUL = no child) into pool; reserve keeps the pointers valid.
TreeNode* build(const vector<int>& level, vector<TreeNode>& pool) {
    if (level.empty()) { return nullptr; }
    pool.reserve(level.size());
    pool.push_back({level[0], nullptr, nullptr});
    // nodes still waiting for their children
    queue<TreeNode*> q;
    q.push(&pool[0]);
    size_t i = 1;
    while (!q.empty() && i < level.size()) {
        TreeNode* cur = q.front();
        q.pop();
        if (level[i] != NUL) {
            pool.push_back({level[i], nullptr, nullptr});
            cur->left = &pool.back();
            q.push(cur->left);
        }
        i++;
        if (i < level.size() && level[i] != NUL) {
            pool.push_back({level[i], nullptr, nullptr});
            cur->right = &pool.back();
            q.push(cur->right);
        }
        i++;
    }
    return &pool[0];
}

void test(const string& label, const vector<int>& level, int want1, int want2) {
    vector<TreeNode> pool;
    TreeNode* root = build(level, pool);
    bool ok = longestConsecutive(root) == want1 && longestConsecutive2(root) == want2;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("up-chain", {1, NUL, 2, NUL, 3, NUL, 3}, 3, 3);
    test("down-steps", {2, NUL, 3, NUL, 2, NUL, 1, NUL, 2, NUL, 2}, 2, 3);
    test("through-root", {2, 1, 3}, 2, 3);
    test("single", {7}, 1, 1);
    test("empty", {}, 0, 0);
}

// follow-up: longest consecutive run in an unsorted array (LC 128) -> hash set; start
//   counting only at values v with no v - 1 in the set. O(n)
