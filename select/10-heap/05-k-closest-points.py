# LC 973 K Closest Points to Origin (Medium). Pattern: size-k max-heap.
# Return the k points [x, y] closest to (0, 0) by Euclidean distance, in any order.

import heapq


# Keep the k nearest in a max-heap so the farthest kept point is evicted first;
# squared distance avoids sqrt. T: O(n log k), S: O(k).
def k_closest(points, k):
    n = len(points)
    # (-squared distance, point index) of the k nearest so far, farthest on top
    heap = []
    for i in range(n):
        x = points[i][0]
        y = points[i][1]
        # negated: heapq is a min-heap
        heapq.heappush(heap, (-(x * x + y * y), i))
        if len(heap) > k:
            heapq.heappop(heap)
    result = []
    while heap:
        neg_dist, i = heapq.heappop(heap)
        result.append(points[i])
    return result


def test(label, points, k, want):
    got = k_closest(points, k)
    got.sort()
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("two-nearest", [[3, 3], [1, 0], [-2, 2], [5, 5]], 2, [[-2, 2], [1, 0]])
    test("all", [[1, 1], [0, 2]], 2, [[0, 2], [1, 1]])
    test("origin", [[0, 0], [-1, -1]], 1, [[0, 0]])


main()

# follow-up: O(n) average -> quickselect on squared distance (same as LC 215), then
#   return the first k
