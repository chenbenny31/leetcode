// LC 105 Construct Binary Tree from Preorder and Inorder Traversal (Medium) — index map
// build the tree from its preorder + inorder; return root
//
// clarify:
// values distinct -> yes, needed for the idx map
// arrays describe same tree -> assume yes
// empty input -> null
// who frees nodes -> caller

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::queue;
using std::string;
using std::unordered_map;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(x), left(l), right(r) {}
};

// builds subtree w/ inorder[inLeft..inRight]; preIdx = next root
TreeNode* dfs(const vector<int>& preorder, int& preIdx, int inLeft, int inRight,
              unordered_map<int, int>& inIdx) {
    if (inLeft > inRight) { return nullptr; }
    TreeNode* root = new TreeNode(preorder[preIdx]);
    preIdx++;
    int rootIdx = inIdx[root->val];
    // preorder = root, left, right: left subtree must consume preIdx first
    root->left = dfs(preorder, preIdx, inLeft, rootIdx - 1, inIdx);
    root->right = dfs(preorder, preIdx, rootIdx + 1, inRight, inIdx);
    return root;
}

// core idea: next preorder value = root; its inorder idx splits range into left / right
// T: O(n), S: O(n)
TreeNode* buildTree(const vector<int>& preorder, const vector<int>& inorder) {
    // inIdx[v] = idx of v in inorder
    unordered_map<int, int> inIdx;
    int n = inorder.size();
    for (int i = 0; i < n; i++) { inIdx[inorder[i]] = i; }
    int preIdx = 0;
    return dfs(preorder, preIdx, 0, n - 1, inIdx);
}

vector<int> toLevels(TreeNode* root) {
    vector<int> out;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* cur = q.front();
        q.pop();
        if (!cur) {
            out.push_back(-1);
            continue;
        }
        out.push_back(cur->val);
        q.push(cur->left);
        q.push(cur->right);
    }
    while (!out.empty() && out.back() == -1) { out.pop_back(); }
    return out;
}

void freeTree(TreeNode* node) {
    if (!node) { return; }
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

void test(const string& label, const vector<int>& pre, const vector<int>& in,
          const vector<int>& want) {
    TreeNode* root = buildTree(pre, in);
    cout << label << (toLevels(root) == want ? " PASS\n" : " FAIL\n");
    freeTree(root);
}

int main() {
    test("full", {1, 2, 4, 5, 3}, {4, 2, 5, 1, 3}, {1, 2, 3, 4, 5});
    test("left-chain", {1, 2, 3}, {3, 2, 1}, {1, 2, -1, 3});
    test("right-chain", {1, 2, 3}, {1, 2, 3}, {1, -1, 2, -1, 3});
    return 0;
}

// follow-up:
// inorder + postorder (LC 106) -> roots from back of postorder, build right before left
// preorder + postorder (LC 889) -> not unique; pre[1] = left root, post idx = left size
// BST from preorder only (LC 1008) -> recurse w/ upper bound, consume while val < bound
// no hash map -> linear search for root in inorder, O(n^2)
// serialize / deserialize (LC 297) -> preorder w/ null markers; one array is enough
