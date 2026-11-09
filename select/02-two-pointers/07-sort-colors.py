# LC 75 Sort Colors (Medium). Pattern: Dutch national flag, 3-way partition.
# nums holds only 0s, 1s and 2s; sort it in place in one pass without a library sort.


# [0, zero_end) = 0s, [zero_end, cur) = 1s, [cur, two_start] unseen,
# (two_start, n-1] = 2s. T: O(n), S: O(1).
def sort_colors(nums):
    zero_end = 0
    cur = 0
    two_start = len(nums) - 1
    while cur <= two_start:
        if nums[cur] == 0:
            # nums[zero_end] is a known 1 (or cur itself), so cur can advance
            nums[zero_end], nums[cur] = nums[cur], nums[zero_end]
            zero_end += 1
            cur += 1
        elif nums[cur] == 2:
            # the value from two_start is unseen: re-check it, so cur stays
            nums[cur], nums[two_start] = nums[two_start], nums[cur]
            two_start -= 1
        else:
            cur += 1


def test(label, nums, want):
    sort_colors(nums)
    print(label, "PASS" if nums == want else "FAIL")


def main():
    test("reversed", [2, 1, 0], [0, 1, 2])
    test("mixed", [1, 2, 0, 2, 1, 0], [0, 0, 1, 1, 2, 2])
    test("swap-in-two", [2, 2, 0], [0, 2, 2])
    test("single", [1], [1])


main()

# follow-up: two passes allowed -> count 0s, 1s, 2s, then overwrite, O(n)
# follow-up: k colors (LintCode 143) -> counting sort O(n + k), or recursive 3-way
#   partition on the middle color, O(n log k)
