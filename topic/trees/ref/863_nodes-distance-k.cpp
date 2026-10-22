// LC 863 All Nodes Distance K in Binary Tree: given a binary tree with unique values, a
// target node and k, return the values of all nodes exactly k edges from the target.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::sort;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

void dfs(TreeNode* node, unordered_map<TreeNode*, TreeNode*>& parents) {
    if (node == nullptr) { return; }
    if (node->left != nullptr) {
        parents[node->left] = node;
        dfs(node->left, parents);
    }
    if (node->right != nullptr) {
        parents[node->right] = node;
        dfs(node->right, parents);
    }
}

// The tree is an undirected graph once each node knows its parent: record parents, then
// BFS k levels out from the target. T: O(n), S: O(n).
vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
    unordered_map<TreeNode*, TreeNode*> parents;   // child -> its parent
    dfs(root, parents);
    unordered_set<TreeNode*> seen = {target};      // nodes already reached
    vector<TreeNode*> cur = {target};              // nodes at the current distance
    for (int dist = 0; dist < k; dist++) {
        vector<TreeNode*> nxt;
        for (TreeNode* node : cur) {
            TreeNode* neighbors[3] = {node->left, node->right, nullptr};
            auto it = parents.find(node);
            if (it != parents.end()) { neighbors[2] = it->second; }
            for (TreeNode* next : neighbors) {
                if (next != nullptr && seen.insert(next).second) { nxt.push_back(next); }
            }
        }
        cur = nxt;
    }
    vector<int> out;
    for (TreeNode* node : cur) {
        out.push_back(node->val);
    }
    return out;
}

void test(const string& label, TreeNode* root, TreeNode* target, int k,
          vector<int> want) {
    vector<int> got = distanceK(root, target, k);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n4(4), n5(5), n6(6), n7(7);
    TreeNode n2(2, &n4, &n5), n3(3, &n6, &n7), n1(1, &n2, &n3);
    test("target-itself", &n1, &n2, 0, {2});
    test("down-and-up", &n1, &n2, 1, {1, 4, 5});
    test("across-parent", &n1, &n2, 2, {3});
    test("too-far", &n1, &n2, 5, {});
}
