# LC 78 Subsets (Medium). Pattern: backtracking, every node of the tree is an answer.
# Return all subsets (the power set) of nums; values are distinct.


# path = chosen values so far; only nums[start..] may still be added
def dfs(nums, start, path, result):
    # copy: path keeps changing after this
    result.append(list(path))
    n = len(nums)
    for i in range(start, n):
        path.append(nums[i])
        dfs(nums, i + 1, path, result)
        path.pop()


# Backtracking: 2^n subsets, copying each is O(n). T: O(n * 2^n), S: O(n) + output.
def subsets(nums):
    result = []
    path = []
    dfs(nums, 0, path, result)
    return result


def test(label, nums, want):
    print(label, "PASS" if subsets(nums) == want else "FAIL")


def main():
    test("two", [1, 2], [[], [1], [1, 2], [2]])
    test("three-unsorted", [3, 1, 2],
         [[], [3], [3, 1], [3, 1, 2], [3, 2], [1], [1, 2], [2]])
    test("empty", [], [[]])


main()

# follow-up: duplicates (LC 90) -> sort, then skip nums[i] == nums[i - 1] when i > start
# follow-up: iterative -> bitmask in range(1 << n), take nums[i] if (mask >> i) & 1
