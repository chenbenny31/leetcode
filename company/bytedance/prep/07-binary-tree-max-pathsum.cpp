// path must go pass root or start at leaf?
// path be single node? yes
// val be neg? yes
// empty tree? no
// overflow n? no
// return -> the sum, path is follow-up

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

// return the best downward chain sum starting at node
int dfs(TreeNode* node, int& best) {
    if (!node) { return 0; }
    int left = max(dfs(node->left, best), 0);
    int right = max(dfs(node->right, best), 0);
    best = max(best, node->val + left + right);
    return node->val + max(left, right);
}

// postrder DFS, T: O(n), S: O(h) recur
int maxPathSum(TreeNode* root) {
    int best = root->val;
    dfs(root, best);
    return best;
}

// follow-up, remember the peak on path and each node's sum
int gainTrack(TreeNode* node, int& best,
             TreeNode*& peak, unordered_map<TreeNode*, int>& gain) {
    if (!node) { return 0; }
    int leftGain = max(gainTrack(node->left, best, peak, gain), 0);
    int rightGain = max(gainTrack(node->right, best, peak, gain), 0);
    if (node->val + leftGain + rightGain > best) {
        best = node->val+ leftGain + rightGain;
        peak = node;
    }
    gain[node] = node->val + max(leftGain, rightGain);
    return gain[node];
}

// followup: collect the chain through peak
vector<int> downChain(TreeNode* node, unordered_map<TreeNode*, int>& gain) {
    vector<int> chain;
    while (node && gain[node] > 0) {
        chain.push_back(node->val);
        int left = node->left ? gain[node->left] : 0;
        int right = node->right ? gain[node->right] : 0;
        if (left >= right) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return chain;
}

// follow-up: rev left chain, peak ,right, T: O(n), S: O(n) of gain
vector<int> maxPathNodes(TreeNode* root) {
    int best = root->val;
    TreeNode* peak = root;
    unordered_map<TreeNode*, int> gain;
    gainTrack(root, best, peak, gain);
    vector<int> path = downChain(peak->left, gain);
    reverse(path.begin(), path.end());
    path.push_back(peak->val);
    vector<int> right = downChain(peak->right, gain);
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
