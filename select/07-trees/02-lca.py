# LC 236 Lowest Common Ancestor of a Binary Tree (Medium). Pattern: post-order return.
# Given two nodes p and q of a binary tree, return their lowest common ancestor
# (a node may be its own ancestor).


class TreeNode:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


# Each call returns p or q if its subtree holds one, else None; the first node that
# gets hits from both sides is the LCA. T: O(n), S: O(h).
def lowest_common_ancestor(root, p, q):
    # reaching p or q stops the descent: the other one below it makes it the LCA anyway
    if root is None or root is p or root is q:
        return root
    left = lowest_common_ancestor(root.left, p, q)
    right = lowest_common_ancestor(root.right, p, q)
    if left and right:
        return root
    if left:
        return left
    return right


def test(label, root, p, q, want):
    ok = lowest_common_ancestor(root, p, q) is want
    print(label, "PASS" if ok else "FAIL")


def main():
    # 1(2(4, 5), 3(6, 7))
    n4 = TreeNode(4)
    n5 = TreeNode(5)
    n6 = TreeNode(6)
    n7 = TreeNode(7)
    n2 = TreeNode(2, n4, n5)
    n3 = TreeNode(3, n6, n7)
    n1 = TreeNode(1, n2, n3)
    test("split-at-root", n1, n4, n7, n1)
    test("p-is-ancestor", n1, n2, n5, n2)
    test("sibling-leaves", n1, n6, n7, n3)


main()

# follow-up: BST (LC 235) -> from the root, both smaller go left, both larger go right,
#   else the current node is the LCA, O(h)
# follow-up: nodes have parent pointers (LC 1650) -> two pointers as in list intersection
