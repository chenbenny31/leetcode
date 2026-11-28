# 16 · Merge k Sorted Lists (ServiceNow phone 2024, Hard) — LC 23
# merge k ascending linked lists into one ascending list, reusing the nodes; return head
#
# clarify:
# empty lists / k = 0 -> skip / return None
# dups -> kept
# reuse nodes or allocate? -> reuse
# ranges -> k <= 1e4, N <= 1e4 nodes total

import heapq


class ListNode:
    def __init__(self, val, next=None):
        self.val = val
        self.next = next


# ==================== solution 1: min-heap of heads (start here) ====================
# also works for streams / iterators

# core idea: the heap holds the cur head of each list; pop the smallest, append it,
# push its next
# T: O(N log k), S: O(k)
def merge_k_lists(lists):
    # heads[i] = next unmerged node of list i
    heads = list(lists)
    # (val, list idx) of each non-empty head; the idx breaks ties, nodes can't compare
    heap = []
    for i in range(len(heads)):
        if heads[i]:
            heapq.heappush(heap, (heads[i].val, i))
    dummy = ListNode(0)
    tail = dummy
    while heap:
        _, i = heapq.heappop(heap)
        tail.next = heads[i]
        tail = heads[i]
        heads[i] = heads[i].next
        # refill from the list just taken from
        if heads[i]:
            heapq.heappush(heap, (heads[i].val, i))
    return dummy.next


# ==================== solution 2: divide and conquer ====================
# merge each half of the lists, then merge the two results; log k levels, no heap

# merges two sorted lists, returns head
def merge_two(left, right):
    dummy = ListNode(0)
    tail = dummy
    while left and right:
        if left.val <= right.val:
            tail.next = left
            left = left.next
        else:
            tail.next = right
            right = right.next
        tail = tail.next
    if left:
        tail.next = left
    else:
        tail.next = right
    return dummy.next


# merges lists[lo..hi] (inclusive), returns head
def merge_range(lists, lo, hi):
    if lo > hi:
        return None
    if lo == hi:
        return lists[lo]
    mid = (lo + hi) // 2
    left = merge_range(lists, lo, mid)
    right = merge_range(lists, mid + 1, hi)
    return merge_two(left, right)


# T: O(N log k), S: O(log k) — recursion
def merge_k_lists_divide(lists):
    return merge_range(lists, 0, len(lists) - 1)


# ==================== tests ====================

# one linked list per row
def build_lists(rows):
    heads = []
    for row in rows:
        dummy = ListNode(0)
        tail = dummy
        for v in row:
            tail.next = ListNode(v)
            tail = tail.next
        heads.append(dummy.next)
    return heads


def to_values(head):
    values = []
    while head:
        values.append(head.val)
        head = head.next
    return values


def test(label, rows, want):
    by_heap = to_values(merge_k_lists(build_lists(rows)))
    by_divide = to_values(merge_k_lists_divide(build_lists(rows)))
    ok = by_heap == want and by_divide == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("three-lists", [[1, 4], [2, 3], [0, 5]], [0, 1, 2, 3, 4, 5])
    test("dups-and-empty", [[1, 1], [], [1]], [1, 1, 1])
    test("single-list", [[2, 6, 9]], [2, 6, 9])
    test("no-lists", [], [])


main()

# follow-up:
# 1. merge two sorted lists (LC 21) -> merge_two above: dummy head, take the smaller head
# 2. k sorted arrays / iterators -> same heap with (value, source idx, position)
# 3. inputs too big for memory -> heap over k file readers, write the output in chunks
# 4. smallest range covering all lists (LC 632) -> same heap + cur max; advance the min
