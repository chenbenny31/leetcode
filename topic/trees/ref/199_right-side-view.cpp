// LC 199 Binary Tree Right Side View: return the values of the nodes you can see when
// standing on the right side of the tree, ordered from top to bottom.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// BFS by level; the last node of each level is the one visible from the right.
// T: O(n), S: O(w) for the widest level w.
vector<int> rightSideView(TreeNode* root) {
    vector<int> view;
    if (root == nullptr) { return view; }
    queue<TreeNode*> q;   // nodes of the current level, then their children
    q.push(root);
    while (!q.empty()) {
        int width = q.size();
        for (int i = 0; i < width; i++) {
            TreeNode* cur = q.front();
            q.pop();
            if (i == width - 1) { view.push_back(cur->val); }
            if (cur->left != nullptr) { q.push(cur->left); }
            if (cur->right != nullptr) { q.push(cur->right); }
        }
    }
    return view;
}

void test(const string& label, TreeNode* root, const vector<int>& want) {
    cout << label << (rightSideView(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode d(4), b(2, &d), c(3), a(1, &b, &c);
    test("left-peeks-out", &a, {1, 3, 4});
    TreeNode single(5);
    test("single", &single, {5});
    test("empty", nullptr, {});
}
