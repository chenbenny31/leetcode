# LC 138 Copy List with Random Pointer (Medium). Pattern: old -> new node hash map.
# Each node has next and random (any node or null). Return a deep copy; no pointer of
# the copy may point into the original.


class Node:
    def __init__(self, val, next=None, random=None):
        self.val = val
        self.next = next
        self.random = random


# Pass 1 creates every copy, pass 2 wires next and random through the map.
# T: O(n), S: O(n).
def copy_random_list(head):
    # original node -> its copy; None -> None
    copy_of = {None: None}
    cur = head
    while cur:
        copy_of[cur] = Node(cur.val)
        cur = cur.next
    cur = head
    while cur:
        copy_of[cur].next = copy_of[cur.next]
        copy_of[cur].random = copy_of[cur.random]
        cur = cur.next
    return copy_of[head]


# Follow-up O(1) extra: weave A -> A' -> B -> B', so X.random's copy is X.random.next,
# then unweave and restore the original. T: O(n), S: O(1).
def copy_random_list_in_place(head):
    cur = head
    while cur:
        copy = Node(cur.val)
        copy.next = cur.next
        cur.next = copy
        cur = copy.next
    cur = head
    while cur:
        if cur.random:
            cur.next.random = cur.random.next
        cur = cur.next.next
    dummy = Node(0)
    # last node of the copied list
    tail = dummy
    cur = head
    while cur:
        copy = cur.next
        cur.next = copy.next
        tail.next = copy
        tail = copy
        cur = cur.next
    return dummy.next


# list -> [val, index its random points to or -1, ...]; -2 if random leaves the list
def encode(head):
    # pos[node] = its position in this list; None -> -1
    pos = {None: -1}
    cur = head
    while cur:
        pos[cur] = len(pos) - 1
        cur = cur.next
    out = []
    cur = head
    while cur:
        out.append(cur.val)
        out.append(pos.get(cur.random, -2))
        cur = cur.next
    return out


# same shape as head, and no node of copy is the original node at that position
def check(head, copy):
    ok = encode(copy) == encode(head)
    orig = head
    while ok and orig:
        ok = orig is not copy
        orig = orig.next
        copy = copy.next
    return ok


# rnd[i] = index node i's random points to, -1 for None
def test(label, vals, rnd):
    # nodes[i] = the i-th node of the original list
    nodes = [Node(v) for v in vals]
    for i in range(len(nodes)):
        if i + 1 < len(nodes):
            nodes[i].next = nodes[i + 1]
        if rnd[i] >= 0:
            nodes[i].random = nodes[rnd[i]]
    head = None
    if nodes:
        head = nodes[0]
    ok = check(head, copy_random_list(head))
    ok = ok and check(head, copy_random_list_in_place(head))
    print(label, "PASS" if ok else "FAIL")


def main():
    test("random-links", [7, 13, 11], [2, -1, 0])
    test("self-random", [5], [0])
    test("dup-values", [3, 3, 3], [-1, 0, -1])
    test("empty", [], [])


main()

# follow-up: clone graph (LC 133) -> same original -> copy map, filled during DFS / BFS
