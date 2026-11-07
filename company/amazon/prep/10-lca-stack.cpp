#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

struct TreeNode {
    int val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    TreeNode(int v) : val(v) {}
};

// DFS post-order
TreeNode* findLca(TreeNode* root, int p, int q) {
    if (root == nullptr || root->val == p || root->val == q) { return root; }
    TreeNode* left = findLca(root->left, p, q);
    TreeNode* right = findLca(root->right, p, q);
    if (left != nullptr && right != nullptr) { return root; }
    return left != nullptr ? left : right;
}

int lowestCommonAncestorRecur(TreeNode* root, int p, int q) {
    return findLca(root, p, q)->val;
}

// follow-up, possible missing p, q
TreeNode* findLcaCount(TreeNode* root, int p, int q, int& found) {
    if (root == nullptr) { return nullptr; }

    TreeNode* left = findLcaCount(root->left, p, q, found);
    TreeNode* right = findLcaCount(root->right, p, q, found);
    if (root->val == p || root->val == q) {
        found++;
        return root;
    }

    if (left != nullptr && right != nullptr) { return root; }
    return left != nullptr ? left : right;
}

TreeNode* lcaIfPresentRecur(TreeNode* root, int p, int q) {
    int found = 0;
    TreeNode* lca = findLcaCount(root, p, q, found);
    int need = p == q ? 1 : 2;
    return found == need ? lca : nullptr;
}

int depth(TreeNode* root, int v, int d) {
    if (root == nullptr) { return -1; }
    if (root->val == v) { return d; }
    int left = depth(root->left, v, d + 1);
    if (left != -1) { return left; }
    return depth(root->right, v, d + 1);
}

int distance(TreeNode* root, int p, int q) {
    TreeNode* lca = findLca(root, p, q);
    return depth(lca, p, 0) + depth(lca, q, 0);
}

// stack DFS records every nodes' parent, put p's ancestors in set
// then climb from q until first node appears in set, T: O(n), S: O(n)
int lowestCommonAncestor(TreeNode* root, int p, int q) {
    unordered_map<TreeNode*, TreeNode*> parent; // node -> node's parent
    parent[root] = nullptr;
    TreeNode* nodeP = nullptr;
    TreeNode* nodeQ = nullptr;
    vector<TreeNode*> stk{root};

    while (!stk.empty()) {
        TreeNode* cur = stk.back();
        stk.pop_back();
        if (cur->val == p) { nodeP = cur; }
        if (cur->val == q) { nodeQ = cur; }

        if (cur->left != nullptr) {
            parent[cur->left] = cur;
            stk.push_back(cur->left);
        }
        if (cur->right != nullptr) {
            parent[cur->right] = cur;
            stk.push_back(cur->right);
        }
    }

    unordered_set<TreeNode*> ancestors;
    for (TreeNode* node = nodeP; node != nullptr; node = parent[node]) {
        ancestors.insert(node);
    }
    TreeNode* node = nodeQ;
    while (!ancestors.count(node)) {
        node = parent[node];
    }
    return node->val;
}

// follow-up: p or q may be missing
TreeNode* lcaIfPresent(TreeNode* root, int p, int q) {
    if (root == nullptr) { return nullptr; }
    unordered_map<TreeNode*, TreeNode*> parent;
    parent[root] = nullptr;
    TreeNode* nodeQ = nullptr;
    TreeNode* nodeP = nullptr;
    vector<TreeNode*> stk{root};

    while (!stk.empty() && (nodeP == nullptr || nodeQ == nullptr)) {
        TreeNode* cur = stk.back();
        stk.pop_back();
        if (cur->val == p) { nodeP = cur; }
        if (cur->val == q) { nodeQ = cur; }

        if (cur->left != nullptr) {
            parent[cur->left] = cur;
            stk.push_back(cur->left);
        }
        if (cur->right != nullptr) {
            parent[cur->right] = cur;
            stk.push_back(cur->right);
        }
    }

    if (nodeP == nullptr || nodeQ == nullptr) { return nullptr; }
    unordered_set<TreeNode*> ancestors;
    for (TreeNode* node = nodeP; node != nullptr; node = parent[node]) {
        ancestors.insert(node);
    }
    TreeNode* node = nodeQ;
    while (!ancestors.count(node)) {
        node = parent[node];
    }
    return node;
}

void test(const string& label, TreeNode* root, int p, int q, int want) {
    cout << label << (lowestCommonAncestor(root, p, q) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a(3), b(5), c(1), d(6), e(2);
    a.left = &b;
    a.right = &c;
    b.left = &d;
    b.right = &e;
    test("split-at-root", &a, 6, 1, 3);
    test("ancestor-of-other", &a, 5, 2, 5);
    test("siblings", &a, 6, 2, 5);
}
