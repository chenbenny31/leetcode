# LC 1293 Shortest Path in a Grid with Obstacles Elimination (Hard). Pattern: BFS on
# (r, c, k) state. m x n grid, 0 = empty, 1 = obstacle; walk 4-directionally from (0, 0)
# to (m-1, n-1) removing up to k obstacles. Return the min steps, or -1 if impossible.

from collections import deque


# BFS over states (row, col, removals left). T: O(m*n*k), S: O(m*n*k).
def shortest_path(grid, k):
    m = len(grid)
    n = len(grid[0])
    dirs = [(1, 0), (-1, 0), (0, 1), (0, -1)]
    # dist[r][c][left] = steps to reach (r, c) with `left` removals unused, -1 = unseen
    dist = []
    for _ in range(m):
        dist.append([[-1] * (k + 1) for _ in range(n)])
    # states in non-decreasing steps
    q = deque([(0, 0, k)])
    dist[0][0][k] = 0
    while q:
        r, c, left = q.popleft()
        if r == m - 1 and c == n - 1:
            return dist[r][c][left]
        for dr, dc in dirs:
            nxt_r = r + dr
            nxt_c = c + dc
            if nxt_r < 0 or nxt_r >= m or nxt_c < 0 or nxt_c >= n:
                continue
            # stepping onto an obstacle uses one removal
            nxt_left = left - grid[nxt_r][nxt_c]
            if nxt_left < 0 or dist[nxt_r][nxt_c][nxt_left] != -1:
                continue
            dist[nxt_r][nxt_c][nxt_left] = dist[r][c][left] + 1
            q.append((nxt_r, nxt_c, nxt_left))
    return -1


def test(label, grid, k, want):
    print(label, "PASS" if shortest_path(grid, k) == want else "FAIL")


def main():
    test("break-wall", [[0, 1, 0], [0, 1, 0], [0, 1, 0]], 1, 4)
    test("walled-off", [[0, 1, 0], [0, 1, 0], [0, 1, 0]], 0, -1)
    test("not-enough", [[0, 1, 1], [1, 1, 0]], 1, -1)
    test("single", [[0]], 0, 0)


main()

# follow-up: prune -> if k >= m + n - 2, return m + n - 2 at once (Manhattan path)
# follow-up: weighted cells -> Dijkstra on the same (r, c, left) state
