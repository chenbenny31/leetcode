# LC 133 Clone Graph (Medium). Pattern: BFS with old -> new hash map.
# Return a deep copy of a connected undirected graph given one of its nodes (or
# None). Each node has a val and a list of neighbors.

from collections import deque


class Node:
    def __init__(self, val):
        self.val = val
        # adjacent nodes
        self.neighbors = []


# BFS; clone a node when first seen, then wire cloned edges. T: O(V+E), S: O(V).
def clone_graph(node):
    if node is None:
        return None
    # original node -> its clone
    copies = {node: Node(node.val)}
    # originals whose edges are not copied yet
    q = deque([node])
    while q:
        cur = q.popleft()
        for nxt in cur.neighbors:
            # clone on first sight: map membership doubles as the visited set
            if nxt not in copies:
                copies[nxt] = Node(nxt.val)
                q.append(nxt)
            copies[cur].neighbors.append(copies[nxt])
    return copies[node]


# BFS from start: one row [val, BFS ids of its neighbors...] per node
def encode(start):
    # ids[node] = order in which BFS first saw it
    ids = {start: 0}
    q = deque([start])
    code = []
    while q:
        cur = q.popleft()
        row = [cur.val]
        for nxt in cur.neighbors:
            if nxt not in ids:
                ids[nxt] = len(ids)
                q.append(nxt)
            row.append(ids[nxt])
        code.append(row)
    return code


def test(label, node):
    copy = clone_graph(node)
    if node is None:
        ok = copy is None
    else:
        # connected: one shared node would pull every original in, so the codes differ
        ok = copy is not node and encode(copy) == encode(node)
    print(label, "PASS" if ok else "FAIL")


def main():
    # 1 - 2
    # |   |
    # 4 - 3
    n1 = Node(1)
    n2 = Node(2)
    n3 = Node(3)
    n4 = Node(4)
    n1.neighbors = [n2, n4]
    n2.neighbors = [n1, n3]
    n3.neighbors = [n2, n4]
    n4.neighbors = [n3, n1]
    test("square", n1)
    solo = Node(7)
    test("single-node", solo)
    test("empty", None)


main()

# follow-up: DFS version -> clone(node): return copies[node] if present; else create and
#   store it in copies before recursing on neighbors (breaks cycles), O(V+E)
# follow-up: list with random pointer (LC 138) -> same old -> new map
