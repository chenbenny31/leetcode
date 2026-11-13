// 126 · Binary Tree Longest Consecutive Sequence I / II — LC 298, 549
// longest path of consecutive values, len in nodes; empty tree -> 0
// (1) LC 298: path goes parent -> child only, each value = prev + 1
// (2) LC 549: path may go child -> parent -> child, all +1 or all -1 along the path
//
// clarify:
// (1) direction -> only +1 going down counts
// (2) bends -> at most once, at the top node
// len -> in nodes, not edges

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

// (1) parent -> child only
// core idea: dfs returns longest +1 run starting at node, going down; best = global max
// T: O(n), S: O(h)
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

// (2) child -> parent -> child
// core idea: dfs returns (inc, dec) = longest downward +1 / -1 paths from node
// path bending at node has inc + dec - 1 nodes (node counted in both)
// T: O(n), S: O(h)
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

// LC level order (NUL = no child) into pool; reserve keeps the pointers valid
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
    test("empty", {}, 0, 0);
}

// follow-up:
// longest consecutive run in array (LC 128) -> hash set; start only at v w/o v - 1
