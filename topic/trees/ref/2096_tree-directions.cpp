// LC 2096 Step-By-Step Directions From a Binary Tree Node to Another: in a binary tree
// with unique values, return the shortest path from startValue to destValue as a string
// of 'L' (to left child), 'R' (to right child) and 'U' (to parent).
#include <iostream>
#include <string>
using std::cout;
using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// Fills path with the moves from node down to target; false if target is not below node.
bool dfs(TreeNode* node, int target, string& path) {
    if (node == nullptr) { return false; }
    if (node->val == target) { return true; }
    path.push_back('L');
    if (dfs(node->left, target, path)) { return true; }
    path.pop_back();
    path.push_back('R');
    if (dfs(node->right, target, path)) { return true; }
    path.pop_back();
    return false;
}

// Root-to-node path strings for both nodes; their common prefix leads to the LCA, so drop
// it. The rest of start's path becomes 'U' moves, the rest of dest's path is walked down.
// T: O(n), S: O(h).
string getDirections(TreeNode* root, int startValue, int destValue) {
    string toStart;   // moves from the root to start
    string toDest;    // moves from the root to dest
    dfs(root, startValue, toStart);
    dfs(root, destValue, toDest);
    int a = toStart.size();
    int b = toDest.size();
    int common = 0;   // length of the shared prefix
    while (common < a && common < b && toStart[common] == toDest[common]) {
        common++;
    }
    return string(a - common, 'U') + toDest.substr(common);
}

void test(const string& label, TreeNode* root, int start, int dest, const string& want) {
    cout << label << (getDirections(root, start, dest) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n4(4), n5(5), n6(6), n7(7);
    TreeNode n2(2, &n4, &n5), n3(3, &n6, &n7), n1(1, &n2, &n3);
    test("via-root", &n1, 4, 7, "UURR");
    test("descend", &n1, 1, 5, "LR");
    test("ascend", &n1, 6, 3, "U");
    test("siblings", &n1, 4, 5, "UR");
}
