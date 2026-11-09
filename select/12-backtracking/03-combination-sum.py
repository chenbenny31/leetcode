# LC 39 Combination Sum (Medium). Pattern: backtracking with reuse (recurse on i).
# Distinct positive candidates; return all unique combinations summing to target, each
# candidate usable any number of times.


# path = chosen values; remain = target - sum(path); start = first index allowed
def dfs(nums, start, remain, path, result):
    if remain == 0:
        # copy: path keeps changing after this
        result.append(list(path))
        return
    n = len(nums)
    for i in range(start, n):
        # sorted: every later candidate is too big as well
        if nums[i] > remain:
            break
        path.append(nums[i])
        # i, not i + 1: the same candidate may be reused
        dfs(nums, i, remain - nums[i], path, result)
        path.pop()


# Sort, then backtrack with pruning. T: O(n^(t/m)) for target t and smallest
# candidate m (max depth t/m), S: O(t/m) + output.
def combination_sum(candidates, target):
    candidates = sorted(candidates)
    result = []
    path = []
    dfs(candidates, 0, target, path, result)
    return result


def test(label, candidates, target, want):
    ok = combination_sum(candidates, target) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("reuse", [3, 2], 6, [[2, 2, 2], [3, 3]])
    test("ones-or-five", [5, 1], 5, [[1, 1, 1, 1, 1], [5]])
    test("none", [4], 3, [])


main()

# follow-up: each used once, dups in input (LC 40) -> recurse on i + 1 and skip
#   nums[i] == nums[i - 1] when i > start
# follow-up: count only (LC 518) -> dp[s] += dp[s - c] with coins in the outer loop
