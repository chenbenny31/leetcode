// ByteDance (hack2hire) / Jianzhi Offer 26: is tree B a substructure of tree A? True if
// some node of A matches B as a structural prefix (A may have extra descendants).
// An empty B is never a substructure.
#include <iostream>
#include <string>

using std::cout;
using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// does B fit at a, rooted together? T: O(|B|), S: O(h).
bool matches(TreeNode* a, TreeNode* b) {
    // B ran out: everything of B matched, A's extra nodes are allowed
    if (!b) { return true; }
    if (!a || a->val != b->val) { return false; }
    return matches(a->left, b->left) && matches(a->right, b->right);
}

// Try matches at every node of A. T: O(|A| * |B|), S: O(h).
bool isSubStructure(TreeNode* A, TreeNode* B) {
    if (!A || !B) { return false; }
    return matches(A, B) || isSubStructure(A->left, B) || isSubStructure(A->right, B);
}

void test(const string& label, TreeNode* A, TreeNode* B, bool want) {
    cout << label << (isSubStructure(A, B) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    //       3          4
    //     4   5       /
    //    1 2         1
    TreeNode a1{1, nullptr, nullptr};
    TreeNode a2{2, nullptr, nullptr};
    TreeNode a4{4, &a1, &a2};
    TreeNode a5{5, nullptr, nullptr};
    TreeNode a3{3, &a4, &a5};
    TreeNode b1{1, nullptr, nullptr};
    TreeNode b4{4, &b1, nullptr};
    test("prefix-match", &a3, &b4, true);
    TreeNode c2{2, nullptr, nullptr};
    TreeNode c3{3, nullptr, nullptr};
    TreeNode c1{1, &c2, &c3};
    TreeNode d2{2, nullptr, nullptr};
    TreeNode d4{4, nullptr, nullptr};
    TreeNode d1{1, &d2, &d4};
    test("value-differs", &c1, &d1, false);
    test("empty-b", &a3, nullptr, false);
}

// follow-up: exact subtree (LC 572) -> matches() returns a == b when either is null
//   (both must end together)
// follow-up: subtree in O(|A| + |B|) -> preorder serialize with null markers, each token
//   prefixed by ',' (",1,2,#"), then KMP search
// follow-up: deep / skewed tree -> iterative traversal with an explicit stack
