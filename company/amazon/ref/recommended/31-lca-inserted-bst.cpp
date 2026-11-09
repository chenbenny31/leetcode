// LCA in an inserted BST: insert the distinct values of insertionOrder into a BST from
// left to right, then return the value of the lowest common ancestor of p and q
// (a node may be its own ancestor).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;
using std::unordered_map;

// standard recursive BST insert, with child maps instead of node structs
void insert(int cur, int v, unordered_map<int, int>& leftChild,
            unordered_map<int, int>& rightChild) {
    unordered_map<int, int>& child = v < cur ? leftChild : rightChild;
    if (!child.count(cur)) {
        child[cur] = v;
        return;
    }
    insert(child[cur], v, leftChild, rightChild);
}

// Build the BST with child maps, then walk down from the root: the LCA is the first
// node whose value lies between p and q. T: O(n h) to build (h = height), S: O(n).
int bstLowestCommonAncestor(const vector<int>& insertionOrder, int p, int q) {
    int n = insertionOrder.size();
    // leftChild[v] / rightChild[v] = value of v's left / right child
    unordered_map<int, int> leftChild;
    unordered_map<int, int> rightChild;
    int root = insertionOrder[0];
    for (int i = 1; i < n; i++) {
        insert(root, insertionOrder[i], leftChild, rightChild);
    }
    int lo = min(p, q);
    int hi = max(p, q);
    int cur = root;
    while (cur < lo || cur > hi) {
        if (cur > hi) {
            cur = leftChild[cur];
        } else {
            cur = rightChild[cur];
        }
    }
    return cur;
}

// improved: the LCA is the first inserted value in [min(p, q), max(p, q)]. All values in
// that range follow the same path until the first of them is inserted and splits them.
// T: O(n), S: O(1).
int bstLcaByOrder(const vector<int>& insertionOrder, int p, int q) {
    int lo = min(p, q);
    int hi = max(p, q);
    for (int v : insertionOrder) {
        if (v >= lo && v <= hi) { return v; }
    }
    return -1;
}

void test(const string& label, const vector<int>& order, int p, int q, int want) {
    bool ok = bstLowestCommonAncestor(order, p, q) == want &&
              bstLcaByOrder(order, p, q) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("split", {4, 2, 6, 1, 3}, 1, 3, 2);
    test("root", {4, 2, 6, 1, 3}, 1, 6, 4);
    test("ancestor", {4, 2, 6, 1, 3}, 2, 3, 2);
    test("sorted-chain", {1, 2, 3}, 3, 2, 2);
}
