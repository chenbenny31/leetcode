// path from root to leaf: yes
// val or target be neg? yes, early exit
// empty tree? yes
// output order? any
// dup paths? each path is unq
// overflow: no

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

// path = vals from root to node, remain = target - sum above
void dfs(TreeNode* node, int remain, vector<int>& path, vector<vector<int>>& result) {
    if (!node) { return; }
    path.push_back(node->val);
    remain -= node->val;
    if (!node->left && !node->right && remain == 0) { // isLeaf && pathSum==target
        result.push_back(path);
    }
    dfs(node->left, remain, path, result);
    dfs(node->right, remain, path, result);
    path.pop_back();
}

// DFS + backtracking, T: O(nh) each hit copies a path of h, S: O(h)
vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
    vector<vector<int>> result;
    vector<int> path;
    dfs(root, targetSum, path, result);
    return result;
}

void test(const string& label, TreeNode* root, int targetSum,
          const vector<vector<int>>& want) {
    cout << label << (pathSum(root, targetSum) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    //         5
    //      4     8
    //    11    13  4
    //   7  2      5  1
    TreeNode n7{7};
    TreeNode n2{2};
    TreeNode n13{13};
    TreeNode n5{5};
    TreeNode n1{1};
    TreeNode n11{11, &n7, &n2};
    TreeNode n4b{4, &n5, &n1};
    TreeNode n4{4, &n11};
    TreeNode n8{8, &n13, &n4b};
    TreeNode root{5, &n4, &n8};
    test("lc ex1", &root, 22, {{5, 4, 11, 2}, {5, 8, 4, 5}});

    TreeNode m3{-2};
    TreeNode m2{2, &m3};
    TreeNode m1{1, &m2};
    test("hit before leaf", &m1, 3, {});

    TreeNode one{1};
    test("single node", &one, 1, {{1}});
}
