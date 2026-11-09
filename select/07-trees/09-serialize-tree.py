# LC 297 Serialize and Deserialize Binary Tree (Hard). Pattern: preorder + null markers.
# Design a Codec that turns a binary tree into a string and rebuilds the same tree from
# that string. Values may be negative.


class TreeNode:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


# Preorder, comma-separated, "#" for null: the null markers fix the shape, so one
# traversal is enough, e.g. "1,#,2,#,#". T: O(n), S: O(n) for both directions.
class Codec:
    def serialize(self, root):
        # preorder tokens, "#" for null
        out = []
        self.write(root, out)
        return ",".join(out)

    def deserialize(self, data):
        # preorder tokens, "#" for null
        tokens = data.split(",")
        # next token to read, shared by every call
        pos = 0

        # each call consumes exactly its own subtree's tokens
        def dfs():
            nonlocal pos
            tok = tokens[pos]
            pos += 1
            if tok == "#":
                return None
            node = TreeNode(int(tok))
            node.left = dfs()
            node.right = dfs()
            return node

        return dfs()

    def write(self, node, out):
        if node is None:
            out.append("#")
            return
        out.append(str(node.val))
        self.write(node.left, out)
        self.write(node.right, out)


def test(label, root, want):
    codec = Codec()
    copy = codec.deserialize(want)
    ok = codec.serialize(root) == want and codec.serialize(copy) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    a2 = TreeNode(2)
    a3 = TreeNode(3)
    a1 = TreeNode(1, a2, a3)
    test("full", a1, "1,2,#,#,3,#,#")
    b2 = TreeNode(2)
    b1 = TreeNode(1, None, b2)
    test("right-chain", b1, "1,#,2,#,#")
    c4 = TreeNode(-4)
    c120 = TreeNode(120)
    c0 = TreeNode(0, c4, c120)
    test("negative-multi-digit", c0, "0,-4,#,#,120,#,#")
    test("empty", None, "#")


main()

# follow-up: BST only (LC 449) -> preorder without "#", rebuild with (lo, hi) bounds
# follow-up: LeetCode's level-order format -> BFS writing "#" for null children, read
#   back with a deque, attaching two tokens per popped node
