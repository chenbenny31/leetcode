// 08 · BST Node Distance (Cisco OA, Medium) — BST insert + walk down from the LCA
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
#include <deque>

using std::cout;
using std::string;
using std::vector;
using std::deque;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// T: O(h), S: O(1)
TreeNode* insert(TreeNode* root, TreeNode* node) {
    if (!root) { return node; }
    TreeNode* cur = root;
    while (true) {
        if (node->val < cur->val) {
            if (!cur->left) { cur->left = node; break; }
            cur = cur->left;
        } else {
            if (!cur->right) { cur->right = node; break; }
            cur = cur->right;
        }
    }
    return root;
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
    // owns the nodes; a deque never moves its elements, so the pointers stay valid
    deque<TreeNode> pool;
    TreeNode* root = nullptr;
    for (int v : vals) {
        pool.push_back(TreeNode(v));
        root = insert(root, &pool.back());
    }
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
// plain binary tree, not a BST (LC 1740) -> LCA by post-order (select/07-trees/02-lca)
// nodes w/ new instead of a pool -> post-order free at the end
// many queries on one tree -> store depth + parent per node, climb the deeper one first
// only the LCA (LC 235) -> the same split walk, return lca
// avoid O(n^2) build on sorted input -> ask if a balanced BST is allowed
