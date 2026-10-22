// LC 437 Path Sum III: count the downward paths (parent to child, need not start at the
// root or end at a leaf) in a binary tree whose values add up to targetSum.
#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::unordered_map;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// sum = prefix sum root..node. A path ending here with the target starts just below an
// ancestor whose prefix is sum - target. seen is backtracked so it only holds the path.
int dfs(TreeNode* node, long long sum, int target, unordered_map<long long, int>& seen) {
    if (node == nullptr) { return 0; }
    sum += node->val;
    auto it = seen.find(sum - target);
    int count = it != seen.end() ? it->second : 0;
    seen[sum]++;
    count += dfs(node->left, sum, target, seen);
    count += dfs(node->right, sum, target, seen);
    seen[sum]--;
    return count;
}

// Prefix sums along the current root path in a hash map. T: O(n), S: O(h).
int pathSum(TreeNode* root, int targetSum) {
    unordered_map<long long, int> seen;   // prefix sum -> times it occurs on this path
    seen[0] = 1;                          // the empty prefix, for paths starting at root
    return dfs(root, 0, targetSum, seen);
}

void test(const string& label, TreeNode* root, int target, int want) {
    cout << label << (pathSum(root, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n2(2), n4(4), n3(3, &n2), nm2(-2, nullptr, &n4), n5(5, &n3, &nm2);
    test("not-from-root", &n5, 5, 2);
    test("with-negative", &n5, 3, 2);
    TreeNode m1(-1), p1(1, &m1);
    test("zero-sum", &p1, 0, 1);
    test("empty", nullptr, 1, 0);
}
