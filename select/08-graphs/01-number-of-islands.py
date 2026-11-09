# LC 200 Number of Islands (Medium). Pattern: grid BFS flood fill.
# grid of '1' (land) and '0' (water); count islands, where an island is land connected
# horizontally or vertically.

from collections import deque


# BFS from each unvisited land cell, sinking its island to '0'. T: O(m*n), S: O(m*n).
def num_islands(grid):
    # copy: sinking must not change the caller's grid
    grid = [list(row) for row in grid]
    m = len(grid)
    n = len(grid[0])
    dirs = [(1, 0), (-1, 0), (0, 1), (0, -1)]
    islands = 0
    for r in range(m):
        for c in range(n):
            if grid[r][c] != "1":
                continue
            islands += 1
            # land cells of this island still to expand
            q = deque([(r, c)])
            # mark on push, so no cell is queued twice
            grid[r][c] = "0"
            while q:
                cur_r, cur_c = q.popleft()
                for dr, dc in dirs:
                    nxt_r = cur_r + dr
                    nxt_c = cur_c + dc
                    if nxt_r < 0 or nxt_r >= m or nxt_c < 0 or nxt_c >= n:
                        continue
                    if grid[nxt_r][nxt_c] != "1":
                        continue
                    grid[nxt_r][nxt_c] = "0"
                    q.append((nxt_r, nxt_c))
    return islands


def test(label, grid, want):
    print(label, "PASS" if num_islands(grid) == want else "FAIL")


def main():
    test("two-islands", [["1", "1", "0"],
                         ["0", "0", "0"],
                         ["0", "1", "1"]], 2)
    test("u-shape", [["1", "0", "1"],
                     ["1", "0", "1"],
                     ["1", "1", "1"]], 1)
    test("diagonal", [["1", "0"], ["0", "1"]], 2)
    test("all-water", [["0"]], 0)


main()

# follow-up: max area (LC 695) -> count cells popped per BFS, keep the max
# follow-up: land added one cell at a time (LC 305) -> union-find on id r * n + c;
#   count += 1 per add, count -= 1 per successful union; O(m*n + k * alpha)
