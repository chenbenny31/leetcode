# LC 230 Kth Smallest Element in a BST (Medium). Pattern: iterative inorder, stop early.
# Return the k-th smallest value (1-indexed) in a binary search tree.


class TreeNode:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


# Inorder visits a BST in ascending order; return the k-th visited.
# T: O(h + k), S: O(h).
def kth_smallest(root, k):
    # ancestors whose own value and right subtree are still pending
    stk = []
    cur = root
    while cur or stk:
        while cur:
            stk.append(cur)
            cur = cur.left
        cur = stk.pop()
        k -= 1
        if k == 0:
            return cur.val
        cur = cur.right
    return -1


def test(label, root, k, want):
    print(label, "PASS" if kth_smallest(root, k) == want else "FAIL")


def main():
    # 4(2(1, 3), 6)
    n1 = TreeNode(1)
    n3 = TreeNode(3)
    n2 = TreeNode(2, n1, n3)
    n6 = TreeNode(6)
    n4 = TreeNode(4, n2, n6)
    test("smallest", n4, 1, 1)
    test("middle", n4, 3, 3)
    test("largest", n4, 5, 6)


main()

# follow-up: frequent inserts / deletes -> keep subtree size in each node; go left if
#   k <= left_size, stop if k == left_size + 1, else go right with k - left_size - 1, O(h)
