// 43 · BST Node Distance (Cisco OA, Medium) — BST insert + walk down from the LCA
// insert distinct values in the given order into a BST; return num of edges on the path
// between the nodes holding a and b, -1 if either value is not in the tree
//
// clarify:
// values distinct -> yes
// a == b -> 0
// a or b missing -> -1
// input order -> insert as given, no balancing (sorted input makes a chain)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// T: O(h), S: O(h) — recursion
TreeNode* insert(TreeNode* root, int val) {
    if (!root) { return new TreeNode(val); }
    if (val < root->val) {
        root->left = insert(root->left, val);
    } else {
        root->right = insert(root->right, val);
    }
    return root;
}

void freeTree(TreeNode* root) {
    if (!root) { return; }
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// num of edges from node down to val, -1 if val is not below node
// T: O(h), S: O(1)
int depthBelow(TreeNode* node, int val) {
    int depth = 0;
    while (node) {
        if (val == node->val) { return depth; }
        node = val < node->val ? node->left : node->right;
        depth++;
    }
    return -1;
}

// core idea: LCA = first node where a and b stop going the same way; dist = two depths
// T: O(n * h) to build + O(h) to answer, S: O(n) — h = height, n for sorted input
int bstDistance(const vector<int>& vals, int a, int b) {
    TreeNode* root = nullptr;
    for (int v : vals) { root = insert(root, v); }
    TreeNode* lca = root;
    while (lca) {
        if (a < lca->val && b < lca->val) {
            lca = lca->left;
        } else if (a > lca->val && b > lca->val) {
            lca = lca->right;
        } else {
            break;
        }
    }
    int da = depthBelow(lca, a);
    int db = depthBelow(lca, b);
    freeTree(root);
    if (da == -1 || db == -1) { return -1; }
    return da + db;
}

void test(const string& label, const vector<int>& vals, int a, int b, int want) {
    cout << label << (bstDistance(vals, a, b) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("across-lca", {5, 6, 3, 1, 2, 4}, 2, 4, 3);
    test("ancestor", {5, 6, 3, 1, 2, 4}, 3, 2, 2);
    test("same-node", {5, 6, 3, 1, 2, 4}, 4, 4, 0);
    test("missing", {5, 6, 3, 1, 2, 4}, 2, 9, -1);
    return 0;
}

// follow-up:
// 1. plain binary tree, not a BST (LC 1740) -> LCA by post-order (select/07-trees/02-lca)
// 2. deep chain (sorted input, n ~ 1e5) -> iterative insert to avoid stack overflow
// 3. many queries -> store depth + parent per node, lift the deeper one first
// 4. only the LCA (LC 235) -> the same split walk, return lca
// 5. avoid O(n^2) build on sorted input -> ask if a balanced BST is allowed
