# LC 994 Rotting Oranges (Medium). Pattern: multi-source BFS by layers.
# grid of 0 (empty), 1 (fresh), 2 (rotten); each minute fresh oranges 4-adjacent to a
# rotten one rot. Return minutes until no fresh orange remains, or -1 if impossible.

from collections import deque


# BFS from all rotten oranges at once, one layer per minute. T: O(m*n), S: O(m*n).
def oranges_rotting(grid):
    # copy: rotting must not change the caller's grid
    grid = [list(row) for row in grid]
    m = len(grid)
    n = len(grid[0])
    dirs = [(1, 0), (-1, 0), (0, 1), (0, -1)]
    # oranges that rotted in the latest minute
    q = deque()
    fresh = 0
    for r in range(m):
        for c in range(n):
            if grid[r][c] == 2:
                q.append((r, c))
            if grid[r][c] == 1:
                fresh += 1
    minutes = 0
    # stop once fresh hits 0, so the last layer does not count an extra minute
    while q and fresh > 0:
        minutes += 1
        level_size = len(q)
        for _ in range(level_size):
            cur_r, cur_c = q.popleft()
            for dr, dc in dirs:
                nxt_r = cur_r + dr
                nxt_c = cur_c + dc
                if nxt_r < 0 or nxt_r >= m or nxt_c < 0 or nxt_c >= n:
                    continue
                if grid[nxt_r][nxt_c] != 1:
                    continue
                grid[nxt_r][nxt_c] = 2
                fresh -= 1
                q.append((nxt_r, nxt_c))
    if fresh == 0:
        return minutes
    return -1


def test(label, grid, want):
    print(label, "PASS" if oranges_rotting(grid) == want else "FAIL")


def main():
    test("spread", [[2, 1, 1], [0, 1, 0]], 2)
    test("two-sources", [[2, 1, 1, 1, 2]], 2)
    test("unreachable", [[2, 0, 1]], -1)
    test("no-fresh", [[0, 2]], 0)


main()

# follow-up: nearest-source distance (LC 542 / LC 286) -> same multi-source BFS, dist
#   set on push, O(m*n)
# follow-up: per-cell rot delays (weighted) -> multi-source Dijkstra on time
