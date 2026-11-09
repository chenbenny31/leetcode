# LC 41 First Missing Positive (Hard). Pattern: cyclic sort, index as hash.
# Return the smallest positive integer missing from unsorted nums, in O(n) time and
# O(1) extra space.


# Answer is in 1..n+1, so put each value v in 1..n at slot v - 1, then find the first
# slot that misfits. T: O(n), S: O(1).
def first_missing_positive(nums):
    n = len(nums)
    for i in range(n):
        # each swap puts one value home for good: at most n swaps in total;
        # the home-slot check also stops a duplicate from swapping forever
        while 1 <= nums[i] <= n and nums[nums[i] - 1] != nums[i]:
            # save the index first: in nums[i], nums[nums[i] - 1] = ... the left side
            # assigns nums[i] first, so the second target would use the new value
            home = nums[i] - 1
            nums[i], nums[home] = nums[home], nums[i]
    for i in range(n):
        if nums[i] != i + 1:
            return i + 1
    return n + 1


def test(label, nums, want):
    got = first_missing_positive(nums)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("gap", [2, -5, 1, 4], 3)
    test("all-present", [3, 1, 2], 4)
    test("too-big", [5, 6], 1)
    test("dups", [1, 1], 2)


main()

# follow-up: why not a hash set -> O(n) space; the array itself is the n-slot set
# follow-up: alternative marking -> replace non-positives with n + 1, then mark value v
#   present by making nums[v - 1] negative; first positive slot is the answer
