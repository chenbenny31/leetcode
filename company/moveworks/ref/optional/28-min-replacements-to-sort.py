# 28 · Minimum Replacements To Sort The Array (Moveworks OA, Hard) — LC 2366, greedy
# one operation replaces x by two positive ints summing to x; return the min operations
# that make the array non-decreasing. [3, 9, 3] -> 2, [3, 4, 3] -> 2
#
# clarify:
# values positive, up to 1e9; n up to 1e5 -> big answers are fine in Python
# pieces stay in place (x -> a, b at x's spot) -> yes
# already sorted -> 0


# core idea: the last element never needs splitting; walk right to left with prev = the
# smallest value allowed after x. Split x into the fewest pieces <= prev:
# parts = ceil(x / prev); make the pieces as even as possible so the first (smallest)
# piece, x // parts, is as large as possible -> it becomes the new prev
# T: O(n), S: O(1)
def minimum_replacement(nums):
    ops = 0
    prev = nums[-1]
    for x in reversed(nums[:-1]):
        if x <= prev:
            prev = x
            continue
        parts = (x + prev - 1) // prev
        ops += parts - 1
        prev = x // parts
    return ops


def test(label, nums, want):
    ok = minimum_replacement(nums) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("uneven-split", [10, 3], 3)
    test("split-4", [3, 4, 3], 2)
    test("sorted", [1, 2, 3], 0)
    test("chain", [12, 9, 7, 6, 17, 19, 21], 6)


main()

# follow-up:
# 1. why x // parts -> an even split keeps the smallest piece as large as possible
# 2. why right to left -> the last element is never split, so it fixes the first bound
# 3. return the final array -> push pieces (x // parts, plus 1 on the last x % parts ones)
