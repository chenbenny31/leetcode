# LC 55 Jump Game (Medium). Pattern: greedy farthest reach (LC 45: implicit BFS layers).
# nums[i] = max jump length from index i; starting at index 0, can you reach the last
# index? LC 45: the last index is always reachable; return the min number of jumps.


# Track the farthest reachable index; fail once i lies beyond it. T: O(n), S: O(1).
def can_jump(nums):
    n = len(nums)
    # farthest index reachable from nums[0..i)
    far = 0
    for i in range(n):
        if i > far:
            return False
        far = max(far, i + nums[i])
    return True


# LC 45: implicit BFS; one layer = indices reachable with the same jump count.
# T: O(n), S: O(1).
def jump(nums):
    n = len(nums)
    jumps = 0
    # last index reachable with `jumps` jumps
    end = 0
    # farthest index reachable with jumps + 1
    far = 0
    # stop before the last index: standing on it needs no further jump
    for i in range(n - 1):
        far = max(far, i + nums[i])
        if i == end:
            jumps += 1
            end = far
    return jumps


def test(label, nums, want):
    print(label, "PASS" if can_jump(nums) == want else "FAIL")


def test_jump(label, nums, want):
    print(label, "PASS" if jump(nums) == want else "FAIL")


def main():
    test("over-zero", [2, 0, 0], True)
    test("stuck-on-zero", [1, 0, 1], False)
    test("single", [0], True)
    test_jump("jump-skip-short", [1, 3, 1, 1, 1], 2)
    test_jump("jump-one-hop", [4, 1, 1, 1, 1], 1)
    test_jump("jump-single", [0], 0)


main()

# follow-up: jump +/- arr[i], reach any 0 (LC 1306) -> BFS/DFS with visited, O(n)
# follow-up: return the jump path (LC 45) -> remember the i that set far for each layer
