# LC 239 Sliding Window Maximum (Hard). Pattern: monotonic decreasing deque.
# A window of size k slides over nums one step at a time; return the max of each
# window.

from collections import deque


# Deque holds indices whose values decrease front to back; front is the window max.
# T: O(n), S: O(k).
def max_sliding_window(nums, k):
    # indices in the window, values strictly decreasing front to back
    dq = deque()
    result = []
    for i in range(len(nums)):
        # front index has left the window
        if dq and dq[0] <= i - k:
            dq.popleft()
        # a smaller older value can never be a max while nums[i] is in the window
        while dq and nums[dq[-1]] <= nums[i]:
            dq.pop()
        dq.append(i)
        if i >= k - 1:
            result.append(nums[dq[0]])
    return result


def test(label, nums, k, want):
    got = max_sliding_window(nums, k)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("max-expires", [4, 2, 12, 3], 2, [4, 12, 12])
    test("decreasing", [5, 3, 1], 2, [5, 3])
    test("dups", [2, 2, 1], 2, [2, 2])
    test("k-equals-n", [1, 3, 2], 3, [3])


main()

# follow-up: simpler first answer -> heapq of (-value, index) as a max-heap, lazily pop
#   expired tops, O(n log n)
# follow-up: max - min <= limit (LC 1438) -> max_dq + min_dq; shrink left while the
#   fronts differ by > limit, O(n)
