# LC 105 Construct Binary Tree from Preorder and Inorder Traversal (Medium).
# Pattern: recursion with an inorder index map.
# Build the tree from its preorder and inorder traversals; all values are distinct.

from collections import deque


class TreeNode:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


# Preorder gives the root, its inorder position splits left / right. T: O(n), S: O(n).
def build_tree(preorder, inorder):
    # in_idx[v] = index of v in inorder
    in_idx = {}
    for i in range(len(inorder)):
        in_idx[inorder[i]] = i
    # next root in preorder, shared by every call
    pre_idx = 0

    # builds the subtree whose inorder is inorder[in_left..in_right]
    def dfs(in_left, in_right):
        nonlocal pre_idx
        if in_left > in_right:
            return None
        root = TreeNode(preorder[pre_idx])
        pre_idx += 1
        root_idx = in_idx[root.val]
        # preorder is root, left, right: the left subtree must consume pre_idx first
        root.left = dfs(in_left, root_idx - 1)
        root.right = dfs(root_idx + 1, in_right)
        return root

    return dfs(0, len(inorder) - 1)


# level order with -1 for a missing child, trailing -1s trimmed
def to_levels(root):
    out = []
    q = deque([root])
    while q:
        cur = q.popleft()
        if cur is None:
            out.append(-1)
            continue
        out.append(cur.val)
        q.append(cur.left)
        q.append(cur.right)
    while out and out[-1] == -1:
        out.pop()
    return out


def test(label, pre, inorder, want):
    root = build_tree(pre, inorder)
    print(label, "PASS" if to_levels(root) == want else "FAIL")


def main():
    test("full", [1, 2, 4, 5, 3], [4, 2, 5, 1, 3], [1, 2, 3, 4, 5])
    test("left-chain", [1, 2, 3], [3, 2, 1], [1, 2, -1, 3])
    test("right-chain", [1, 2, 3], [1, 2, 3], [1, -1, 2, -1, 3])


main()

# follow-up: inorder + postorder (LC 106) -> take roots from the back of postorder and
#   build the right subtree before the left
