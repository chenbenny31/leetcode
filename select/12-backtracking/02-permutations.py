# LC 46 Permutations (Medium). Pattern: backtracking with a used[] array.
# Return all permutations of distinct integers nums, in any order.


# path = permutation prefix; used[i] = nums[i] is already in path
def dfs(nums, used, path, result):
    n = len(nums)
    if len(path) == n:
        # copy: path keeps changing after this
        result.append(list(path))
        return
    for i in range(n):
        if used[i]:
            continue
        used[i] = True
        path.append(nums[i])
        dfs(nums, used, path, result)
        path.pop()
        used[i] = False


# Each level picks any unused value. T: O(n * n!), S: O(n) + output.
def permute(nums):
    result = []
    used = [False] * len(nums)
    path = []
    dfs(nums, used, path, result)
    return result


def test(label, nums, want):
    got = permute(nums)
    got.sort()
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("two", [2, 1], [[1, 2], [2, 1]])
    test("three", [3, 1, 2],
         [[1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1], [3, 1, 2], [3, 2, 1]])
    test("single", [7], [[7]])


main()

# follow-up: duplicates (LC 47) -> sort; skip i when nums[i] == nums[i - 1] and
#   not used[i - 1] (equal values are placed left to right only)
# follow-up: next permutation (LC 31) -> rightmost ascent, swap with next larger, reverse
