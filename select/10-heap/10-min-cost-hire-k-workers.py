# LC 857 Minimum Cost to Hire K Workers (Hard). Pattern: sort by ratio + max-heap.
# Hire exactly k workers; pay is proportional to quality within the group and each
# worker gets at least their wage. Return the minimum total pay.

import heapq


# The group's rate is its largest wage / quality. Fix it at each worker in ratio order;
# cost = ratio * sum of qualities, so keep the k smallest qualities.
# T: O(n log n), S: O(n).
def mincost_to_hire_workers(quality, wage, k):
    n = len(quality)
    # (wage / quality, quality), lowest ratio first after sorting
    workers = []
    for i in range(n):
        workers.append((wage[i] / quality[i], quality[i]))
    workers.sort()
    # -quality of each worker in the group; top = largest quality, dropped first
    heap = []
    # sum of qualities in the heap
    sum_quality = 0
    best = float("inf")
    for ratio, qual in workers:
        heapq.heappush(heap, -qual)
        sum_quality += qual
        if len(heap) > k:
            largest = -heapq.heappop(heap)
            sum_quality -= largest
        # ratio is the largest so far, so everyone is paid at least their wage
        if len(heap) == k:
            best = min(best, ratio * sum_quality)
    return best


def test(label, quality, wage, k, want):
    ok = abs(mincost_to_hire_workers(quality, wage, k) - want) < 1e-9
    print(label, "PASS" if ok else "FAIL")


def main():
    test("k-one", [2, 5], [8, 10], 1, 8.0)
    test("hire-all", [1, 2, 3], [3, 2, 3], 3, 18.0)
    test("drop-big-quality", [1, 10, 1], [1, 10, 3], 2, 6.0)
    test("fraction", [3, 2], [4, 1], 2, 20.0 / 3)


main()

# follow-up: why the heap pops the largest quality -> with the rate fixed, the cheapest
#   k workers are the k smallest qualities among those with ratio <= rate
# follow-up: avoid floating point in the sort -> key=Fraction(wage, quality), or
#   cross-multiply w1 * q2 < w2 * q1 via functools.cmp_to_key
