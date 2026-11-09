// LC 437 Path Sum III (Medium). Pattern: prefix sums on the root path + backtracking.
// Count downward paths (parent to child, any start, any end) whose values sum to
// targetSum. Values may be negative.

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
    TreeNode(int x, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(x), left(l), right(r) {}
};

// prefix = sum root..node; a path ending here hits the target if it starts just below
// an ancestor whose prefix is prefix - target
int dfs(TreeNode* node, long long prefix, int target,
        unordered_map<long long, int>& prefixCount) {
    if (!node) { return 0; }
    prefix += node->val;
    auto it = prefixCount.find(prefix - target);
    int count = it != prefixCount.end() ? it->second : 0;
    prefixCount[prefix]++;
    count += dfs(node->left, prefix, target, prefixCount);
    count += dfs(node->right, prefix, target, prefixCount);
    // backtrack: the map must hold only the current root path
    prefixCount[prefix]--;
    return count;
}

// Subarray-sum-equals-k on every root-to-node path. T: O(n), S: O(h).
int pathSum(TreeNode* root, int targetSum) {
    // prefix sum -> times it occurs on the current root path
    unordered_map<long long, int> prefixCount;
    // the empty prefix, for paths starting at the root
    prefixCount[0] = 1;
    return dfs(root, 0, targetSum, prefixCount);
}

void test(const string& label, TreeNode* root, int target, int want) {
    cout << label << (pathSum(root, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    // 5(3(2), -2(_, 4))
    TreeNode n2(2);
    TreeNode n4(4);
    TreeNode n3(3, &n2);
    TreeNode nm2(-2, nullptr, &n4);
    TreeNode n5(5, &n3, &nm2);
    test("not-from-root", &n5, 5, 2);
    test("with-negative", &n5, 3, 2);
    TreeNode m1(-1);
    TreeNode p1(1, &m1);
    test("zero-sum", &p1, 0, 1);
    test("empty", nullptr, 1, 0);
}

// follow-up: brute force first -> from every node DFS down summing, O(n * h)
// follow-up: array version (LC 560) -> same prefix-count map over one pass of the array
