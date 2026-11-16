// Subtree Pair Sum / Product Count (Google, interview report)
// binary tree in a vector pool; take any a from a node's left subtree and any b from
// its right subtree; node is good if every such pair has a + b <= val <= a * b
// return the num of good nodes
// reported: 2027 intern phone (easy); the exact comparison was not in the report
//
// clarify:
// which comparison -> assumed: a + b <= val <= a * b for every pair
// a, b from the whole subtree or only the child -> whole subtree
// leaf, or only one child -> no pair, not good
// negative values -> yes: smallest product is not min * min
// overflow -> products in long long

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;

struct Node {
    int val;
    // child ids, -1 if absent
    int left = -1;
    int right = -1;
};

struct Range {
    // smallest / largest value in a subtree
    long long minVal;
    long long maxVal;
};

// post-order: returns min / max of the subtree at id; adds its good nodes to count
Range dfs(const vector<Node>& tree, int id, int& count) {
    const Node& node = tree[id];
    Range range = {node.val, node.val};
    if (node.left == -1 && node.right == -1) { return range; }
    if (node.left == -1 || node.right == -1) {
        int child = node.left != -1 ? node.left : node.right;
        Range sub = dfs(tree, child, count);
        return {min(range.minVal, sub.minVal), max(range.maxVal, sub.maxVal)};
    }
    Range left = dfs(tree, node.left, count);
    Range right = dfs(tree, node.right, count);
    long long maxSum = left.maxVal + right.maxVal;
    // w/ negatives the smallest product is one of the 4 corner products
    long long minProduct = min(left.minVal * right.minVal, left.minVal * right.maxVal);
    minProduct = min(minProduct, left.maxVal * right.minVal);
    minProduct = min(minProduct, left.maxVal * right.maxVal);
    if (maxSum <= node.val && node.val <= minProduct) { count++; }
    range.minVal = min(range.minVal, min(left.minVal, right.minVal));
    range.maxVal = max(range.maxVal, max(left.maxVal, right.maxVal));
    return range;
}

// core idea: "every pair" only needs the extremes -> each subtree returns (min, max)
// largest sum = max + max; smallest product = min of the 4 corner products
// T: O(n), S: O(h)
int countGoodNodes(const vector<Node>& tree, int root) {
    if (root == -1) { return 0; }
    int count = 0;
    dfs(tree, root, count);
    return count;
}

void test(const string& label, const vector<Node>& tree, int root, int want) {
    cout << label << (countGoodNodes(tree, root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("sum-and-product-hold", {{6, 1, 2}, {2}, {3}}, 0, 1);
    test("sum-too-big", {{4, 1, 2}, {2}, {3}}, 0, 0);
    test("two-levels", {{12, 1, 2}, {4, 3, 4}, {6}, {2}, {2}}, 0, 2);
    test("both-negative", {{0, 1, 2}, {-2}, {-3}}, 0, 1);
    test("mixed-signs", {{7, 1, 2}, {-2, 3, -1}, {3}, {4}}, 0, 0);
    test("one-child", {{5, 1, -1}, {1}}, 0, 0);
    test("empty", {}, -1, 0);
}

// follow-up:
// "some pair" per condition (not the same pair) -> min sum / max product instead
// a + b == val for some pair -> extremes are not enough: value sets, small-to-large merge
// only the two children, not whole subtrees -> no DFS state; compare at each node
// very deep tree -> explicit stack, or process ids in reverse BFS order
