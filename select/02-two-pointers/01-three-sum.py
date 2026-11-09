# LC 15 3Sum (Medium). Pattern: sort + fix one + two pointers.
# Return all unique triplets [a, b, c] of values from nums (distinct indices) with
# a + b + c == 0; no duplicate triplets.


# Sort, fix the smallest value, two pointers on the rest. T: O(n^2), S: O(1) + output.
def three_sum(nums):
    # sorted copy, the caller's list is left as is
    nums = sorted(nums)
    # triplets, each ascending, found in ascending order
    result = []
    n = len(nums)
    for i in range(n - 2):
        # same first value would give the same triplets
        if i > 0 and nums[i] == nums[i - 1]:
            continue
        left = i + 1
        right = n - 1
        while left < right:
            total = nums[i] + nums[left] + nums[right]
            if total < 0:
                left += 1
            elif total > 0:
                right -= 1
            else:
                result.append([nums[i], nums[left], nums[right]])
                left += 1
                right -= 1
                # skip equal values so the same pair is not added again
                while left < right and nums[left] == nums[left - 1]:
                    left += 1
    return result


def test(label, nums, want):
    got = three_sum(nums)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("dup-values", [2, -1, 0, -1, 1], [[-1, -1, 2], [-1, 0, 1]])
    test("all-zeros", [0, 0, 0, 0], [[0, 0, 0]])
    test("dup-pair", [-2, 1, 1, 1, 1], [[-2, 1, 1]])
    test("none", [1, 2, -4], [])


main()

# follow-up: 3Sum closest (LC 16) -> same loop, track total with min |total - target|
# follow-up: 4Sum / kSum (LC 18) -> fix k-2 values with dup skips, two pointers inside,
#   O(n^(k-1)); Python ints don't overflow (C++ needs long long for the sum)
