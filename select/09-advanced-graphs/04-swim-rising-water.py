# LC 778 Swim in Rising Water (Hard). Pattern: Dijkstra on max elevation (bottleneck).
# n x n grid of distinct elevations 0..n*n-1. At time t you can swim between 4-adjacent
# cells of elevation <= t. Return the min t to get from (0, 0) to (n-1, n-1).

import heapq


# Dijkstra where a path's cost is its max elevation. T: O(n^2 log n), S: O(n^2).
def swim_in_water(grid):
    n = len(grid)
    dirs = [(1, 0), (-1, 0), (0, 1), (0, -1)]
    # seen[r][c] = cell already pushed
    seen = [[False] * n for _ in range(n)]
    # (max elevation on the path, r * n + c), smallest first
    heap = [(grid[0][0], 0)]
    seen[0][0] = True
    while heap:
        path_max, cell = heapq.heappop(heap)
        r = cell // n
        c = cell % n
        if r == n - 1 and c == n - 1:
            return path_max
        for dr, dc in dirs:
            nxt_r = r + dr
            nxt_c = c + dc
            if nxt_r < 0 or nxt_r >= n or nxt_c < 0 or nxt_c >= n:
                continue
            if seen[nxt_r][nxt_c]:
                continue
            # mark on push: pops are non-decreasing, so a later push is never better
            seen[nxt_r][nxt_c] = True
            nxt_max = max(path_max, grid[nxt_r][nxt_c])
            heapq.heappush(heap, (nxt_max, nxt_r * n + nxt_c))
    return -1


def test(label, grid, want):
    print(label, "PASS" if swim_in_water(grid) == want else "FAIL")


def main():
    test("single", [[0]], 0)
    test("down-then-right", [[0, 3], [1, 2]], 2)
    test("must-climb", [[0, 5, 1],
                        [6, 7, 2],
                        [8, 4, 3]], 5)


main()

# follow-up: binary search t + BFS through cells <= t -> monotone in t, O(n^2 log n)
# follow-up: union-find by time -> open cells in elevation order, unite open neighbors,
#   stop when (0, 0) and (n-1, n-1) connect; O(n^2 alpha)
