// ByteDance (hack2hire) / LC 124: a path is any sequence of adjacent nodes, each used at
// most once, not necessarily through the root. Return the max path sum (tree non-empty).
// Follow-up: return the node values on the best path, in path order.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::max;
using std::reverse;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// Returns the best downward chain sum starting at node (node plus at most one side).
// best = max path sum seen so far, where a path may bend at any node.
int gain(TreeNode* node, int& best) {
    if (!node) { return 0; }
    // a negative branch is better dropped
    int leftGain = max(gain(node->left, best), 0);
    int rightGain = max(gain(node->right, best), 0);
    // path bending at node uses both sides
    best = max(best, node->val + leftGain + rightGain);
    // a parent can extend only one side
    return node->val + max(leftGain, rightGain);
}

// Postorder gains with a running best. T: O(n), S: O(h) recursion.
int maxPathSum(TreeNode* root) {
    int best = root->val;
    gain(root, best);
    return best;
}

// Follow-up, pass 1: same as gain, but also remember the bend node (peak) of the best
// path and each node's chain gain.
int gainTrack(TreeNode* node, int& best, TreeNode*& peak,
              unordered_map<TreeNode*, int>& gainOf) {
    if (!node) { return 0; }
    int leftGain = max(gainTrack(node->left, best, peak, gainOf), 0);
    int rightGain = max(gainTrack(node->right, best, peak, gainOf), 0);
    if (node->val + leftGain + rightGain > best) {
        best = node->val + leftGain + rightGain;
        peak = node;
    }
    gainOf[node] = node->val + max(leftGain, rightGain);
    return gainOf[node];
}

// Follow-up, pass 2: values of the best downward chain from node, node first; empty
// when the chain's gain is not positive (the parent would have dropped it).
vector<int> downChain(TreeNode* node, unordered_map<TreeNode*, int>& gainOf) {
    vector<int> chain;
    while (node && gainOf[node] > 0) {
        chain.push_back(node->val);
        int leftGain = node->left ? gainOf[node->left] : 0;
        int rightGain = node->right ? gainOf[node->right] : 0;
        if (leftGain >= rightGain) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return chain;
}

// Follow-up: left chain reversed (it runs up to the peak), peak, right chain.
// T: O(n), S: O(n) for gainOf.
vector<int> maxPathNodes(TreeNode* root) {
    int best = root->val;
    TreeNode* peak = root;
    // gainOf[node] = best downward chain sum starting at node
    unordered_map<TreeNode*, int> gainOf;
    gainTrack(root, best, peak, gainOf);
    vector<int> path = downChain(peak->left, gainOf);
    reverse(path.begin(), path.end());
    path.push_back(peak->val);
    vector<int> right = downChain(peak->right, gainOf);
    path.insert(path.end(), right.begin(), right.end());
    return path;
}

void test(const string& label, TreeNode* root, int want, const vector<int>& wantPath) {
    bool ok = maxPathSum(root) == want && maxPathNodes(root) == wantPath;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a2{2, nullptr, nullptr};
    TreeNode a3{3, nullptr, nullptr};
    TreeNode a1{1, &a2, &a3};
    test("through root", &a1, 6, {2, 1, 3});
    //   -10
    //  9    20
    //     15   7
    TreeNode b15{15, nullptr, nullptr};
    TreeNode b7{7, nullptr, nullptr};
    TreeNode b20{20, &b15, &b7};
    TreeNode b9{9, nullptr, nullptr};
    TreeNode b10{-10, &b9, &b20};
    test("skip root", &b10, 42, {15, 20, 7});
    //   -3
    //  -1  -2
    TreeNode c1{-1, nullptr, nullptr};
    TreeNode c2{-2, nullptr, nullptr};
    TreeNode c3{-3, &c1, &c2};
    test("all negative", &c3, -1, {-1});
}
