# LC 283 Move Zeroes (Easy). Pattern: read/write pointers, swap forward.
# Move all 0s in nums to the end in place, keeping the order of the non-zero values.


# Swap each non-zero into the next write slot. T: O(n), S: O(1).
def move_zeroes(nums):
    # nums[0..write) = the non-zeros seen so far, in order
    write = 0
    for read in range(len(nums)):
        if nums[read] != 0:
            nums[write], nums[read] = nums[read], nums[write]
            write += 1


def test(label, nums, want):
    move_zeroes(nums)
    print(label, "PASS" if nums == want else "FAIL")


def main():
    test("mixed", [0, 1, 0, 2, 3], [1, 2, 3, 0, 0])
    test("all-zero", [0, 0], [0, 0])
    test("no-zero", [4, -1], [4, -1])


main()

# follow-up: remove all copies of val (LC 27) -> same write pointer, copy instead of
#   swap, return write as the new length
