# LC 4 Median of Two Sorted Arrays (Hard). Pattern: kth smallest, drop k/2 per step.
# Return the median of sorted arrays a and b (m + n >= 1) in O(log(m + n)).


# kth smallest (1-based) of a and b: compare the next rank/2 elements of each array.
# The block whose last element is smaller lies entirely below the answer, so drop it
# and look for a smaller rank. T: O(log k), S: O(1).
def kth_smallest(a, b, k):
    len_a = len(a)
    len_b = len(b)
    # a[start_a..] and b[start_b..] are still in play
    start_a = 0
    start_b = 0
    # rank of the answer among the elements still in play
    rank = k
    while start_a < len_a and start_b < len_b and rank > 1:
        # block sizes: rank // 2 each, fewer if the array runs out
        take_a = min(rank // 2, len_a - start_a)
        take_b = min(rank // 2, len_b - start_b)
        last_a = a[start_a + take_a - 1]
        last_b = b[start_b + take_b - 1]
        if last_a <= last_b:
            start_a += take_a
            rank -= take_a
        else:
            start_b += take_b
            rank -= take_b
    # one array is used up: the answer sits directly in the other
    if start_a == len_a:
        return b[start_b + rank - 1]
    if start_b == len_b:
        return a[start_a + rank - 1]
    # rank == 1: the smaller of the two heads
    return min(a[start_a], b[start_b])


# Median = average of the ((n+1)//2)-th and ((n+2)//2)-th smallest (same one when n is
# odd). T: O(log(m + n)), S: O(1).
def find_median_sorted_arrays(a, b):
    n = len(a) + len(b)
    lower = kth_smallest(a, b, (n + 1) // 2)
    upper = kth_smallest(a, b, (n + 2) // 2)
    # / is true division in Python 3
    return (lower + upper) / 2


def test(label, a, b, want):
    got = find_median_sorted_arrays(a, b)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("odd-total", [1, 4, 7], [2, 3], 3.0)
    test("even-total", [1, 5], [2, 8], 3.5)
    test("one-empty", [], [1, 2, 3, 4], 2.5)
    test("short-runs-out", [1], [2, 3, 4, 5, 6], 3.5)


main()

# follow-up: O(log min(m, n)) -> binary search cut i in the shorter array,
#   j = (m + n + 1) // 2 - i; valid when a[i-1] <= b[j] and b[j-1] <= a[i]
# follow-up: kth of K sorted arrays -> heapq of (value, array, index), pop k times
