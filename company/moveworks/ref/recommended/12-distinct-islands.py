# 12 · Number Of Distinct Islands, Iterative (ServiceNow 2025 + LC tag, Medium) — LC 694
# grid of 0 / 1; islands are 4-connected 1s; two islands are the same if one is a
# translation of the other (no rotation, no mirror). Count distinct islands
#
# clarify:
# iterative -> the report asked for no recursion: BFS with a deque
# rotations / reflections count as the same -> no (LC 711 is that version)
# empty or all water -> 0
# grid size -> up to 50 x 50 on LC

from collections import deque

DIRS = [(1, 0), (-1, 0), (0, 1), (0, -1)]


# core idea: BFS each island; its shape = sorted offsets of its cells from the start
# cell; a set of shapes removes translated copies
# T: O(R * C * log(R * C)), S: O(R * C)
def num_distinct_islands(grid):
    rows = len(grid)
    cols = len(grid[0])
    # copy, so marking cells does not change the caller's grid
    grid = [row[:] for row in grid]
    # each shape = tuple of sorted (dr, dc) offsets from the island's start cell
    shapes = set()
    for r0 in range(rows):
        for c0 in range(cols):
            if grid[r0][c0] != 1:
                continue
            shape = []
            q = deque([(r0, c0)])
            # mark when pushed, so a cell is never queued twice
            grid[r0][c0] = 0
            while q:
                r, c = q.popleft()
                shape.append((r - r0, c - c0))
                for dr, dc in DIRS:
                    nr = r + dr
                    nc = c + dc
                    if nr < 0 or nr >= rows or nc < 0 or nc >= cols:
                        continue
                    if grid[nr][nc] != 1:
                        continue
                    grid[nr][nc] = 0
                    q.append((nr, nc))
            shape.sort()
            # a list can't go in a set; a tuple can
            shapes.add(tuple(shape))
    return len(shapes)


def test(label, grid, want):
    ok = num_distinct_islands(grid) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("same-domino", [[1, 1, 0, 1, 1]], 1)
    test("rotation-differs", [[1, 1, 0, 1], [0, 0, 0, 1]], 2)
    test("single-cells", [[1, 0, 1]], 1)
    test("water", [[0, 0]], 0)


main()

# follow-up:
# 1. why sort the offsets -> BFS order can differ between copies; sorted offsets don't
# 2. key without sorting -> DFS path str ("d", "r", ...) + a "back" mark per return
# 3. rotations / reflections same (LC 711) -> 8 transforms, normalize, min = key
# 4. count islands only -> select/08-graphs/01-num-of-islands
# 5. no copy of the grid -> a separate visited set of (r, c)
