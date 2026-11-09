# LC 25 Reverse Nodes in k-Group (Hard). Pattern: count, then reverse group by group.
# Reverse the list k nodes at a time; a last group with fewer than k nodes stays as is.
# Only next pointers may change. 1 <= k <= list length.


class ListNode:
    def __init__(self, val, next=None):
        self.val = val
        self.next = next


# Count n, then reverse n // k full groups, each hooked after prev_tail.
# T: O(n), S: O(1).
def reverse_k_group(head, k):
    n = 0
    cur = head
    while cur:
        n += 1
        cur = cur.next
    dummy = ListNode(0)
    dummy.next = head
    # last node of the part already done
    prev_tail = dummy
    for _ in range(n // k):
        group_head = prev_tail.next
        prev = None
        cur = group_head
        for _ in range(k):
            nxt = cur.next
            cur.next = prev
            prev = cur
            cur = nxt
        # prev = new group head, group_head = new group tail, cur = next group's head
        prev_tail.next = prev
        group_head.next = cur
        prev_tail = group_head
    return dummy.next


# links new nodes holding vals in order; returns the head
def build_list(vals):
    dummy = ListNode(0)
    tail = dummy
    for v in vals:
        tail.next = ListNode(v)
        tail = tail.next
    return dummy.next


def to_list(head):
    out = []
    cur = head
    while cur:
        out.append(cur.val)
        cur = cur.next
    return out


def test(label, vals, k, want):
    head = reverse_k_group(build_list(vals), k)
    print(label, "PASS" if to_list(head) == want else "FAIL")


def main():
    test("k-one", [1, 2, 3], 1, [1, 2, 3])
    test("whole-list", [1, 2, 3], 3, [3, 2, 1])
    test("even-groups", [1, 2, 3, 4, 5, 6], 3, [3, 2, 1, 6, 5, 4])
    test("short-tail", [1, 2, 3, 4, 5, 6], 4, [4, 3, 2, 1, 5, 6])


main()

# follow-up: reverse the short tail too -> one more reversal of the last n % k nodes
# follow-up: recursive -> reverse the first k, group_head.next = reverse_k_group(rest, k),
#   O(n / k) stack
