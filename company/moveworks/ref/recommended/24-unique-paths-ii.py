# 24 · Unique Paths II (ServiceNow phone 2024, Medium) — LC 63
# count right / down paths from the top-left to the bottom-right of a grid where
# cells marked 1 are obstacles
#
# clarify:
# start or end blocked -> 0
# moves -> right and down only
# answer size -> fits in int on LC


# core idea: paths to a cell = paths from above + paths from the left; an obstacle has
# 0; keep only one row
# T: O(m * n), S: O(n)
def unique_paths_with_obstacles(grid):
    m = len(grid)
    n = len(grid[0])
    # dp[c] = paths to column c of the cur row; seeded so row 0 starts at 1
    dp = [0] * n
    dp[0] = 1
    for r in range(m):
        for c in range(n):
            if grid[r][c] == 1:
                dp[c] = 0
            elif c > 0:
                # dp[c] still holds the row above; dp[c - 1] is this row's left cell
                dp[c] += dp[c - 1]
    return dp[n - 1]


def test(label, grid, want):
    ok = unique_paths_with_obstacles(grid) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("open", [[0, 0], [0, 0]], 2)
    test("block-forces-path", [[0, 1], [0, 0]], 1)
    test("start-blocked", [[1, 0], [0, 0]], 0)
    test("end-blocked", [[0, 0], [0, 1]], 0)


main()

# follow-up:
# 1. no obstacles (LC 62) -> C(m + n - 2, m - 1), or the same dp
# 2. min path sum (LC 64) -> dp[c] = grid[r][c] + min(dp[c], dp[c - 1])
# 3. return one path -> keep the full 2D dp, walk back from the end
