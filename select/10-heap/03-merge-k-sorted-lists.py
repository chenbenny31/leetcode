# LC 23 Merge k Sorted Lists (Hard). Pattern: min-heap of list heads.
# Merge k sorted linked lists into one sorted list and return its head.

import heapq


class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next


# Min-heap holds the current head of each list; pop the smallest, push its successor.
# T: O(N log k) for N nodes in total, S: O(k).
def merge_k_lists(lists):
    # heads[i] = next unmerged node of list i
    heads = list(lists)
    k = len(heads)
    # (val, list index) of each non-empty list's head, smallest val on top;
    # the index breaks ties, so ListNodes are never compared
    heap = []
    for i in range(k):
        if heads[i]:
            heapq.heappush(heap, (heads[i].val, i))
    dummy = ListNode(0)
    tail = dummy
    while heap:
        val, list_idx = heapq.heappop(heap)
        tail.next = heads[list_idx]
        tail = heads[list_idx]
        heads[list_idx] = heads[list_idx].next
        # refill from the list we just took from
        if heads[list_idx]:
            heapq.heappush(heap, (heads[list_idx].val, list_idx))
    return dummy.next


def test(label, lists, want):
    got = []
    cur = merge_k_lists(lists)
    while cur:
        got.append(cur.val)
        cur = cur.next
    print(label, "PASS" if got == want else "FAIL")


def main():
    a2 = ListNode(4)
    a1 = ListNode(1, a2)
    b2 = ListNode(3)
    b1 = ListNode(2, b2)
    c2 = ListNode(5)
    c1 = ListNode(0, c2)
    test("three-lists", [a1, b1, c1], [0, 1, 2, 3, 4, 5])
    d2 = ListNode(1)
    d1 = ListNode(1, d2)
    e1 = ListNode(1)
    test("dups-and-empty", [d1, None, e1], [1, 1, 1])
    test("no-lists", [], [])


main()

# follow-up: no heap -> divide and conquer: merge lists in pairs, then pairs of
#   results, log k rounds of O(N) each; O(N log k) time, O(1) extra if iterative
