# LC 1 Two Sum (Easy). Pattern: hash map of earlier values.
# Return the indices of the two numbers in nums that add up to target. Exactly one
# answer exists; the same element can't be used twice.


# One pass: look up target - x among earlier values, then record x. T: O(n), S: O(n).
def two_sum(nums, target):
    # seen[value] = index of an earlier element with that value
    seen = {}
    for i in range(len(nums)):
        complement = target - nums[i]
        # look up before insert, so nums[i] never pairs with itself
        if complement in seen:
            return [seen[complement], i]
        seen[nums[i]] = i
    return []


def test(label, nums, target, want):
    got = two_sum(nums, target)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("basic", [2, 7, 5], 12, [1, 2])
    test("not-self", [4, 1, 7], 8, [1, 2])
    test("dup", [5, 2, 5], 10, [0, 2])
    test("negative", [-3, 4, 1], -2, [0, 2])


main()

# follow-up: input sorted (LC 167) -> two pointers from both ends, move the side that
#   makes the sum closer to target, O(1) space
# follow-up: values in a small range [lo, hi] -> seen = [-1] * (hi - lo + 1)
#   indexed by v - lo replaces the dict; check target - v is in range first
