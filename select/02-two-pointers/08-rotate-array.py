# LC 189 Rotate Array (Medium). Pattern: three reversals.
# Rotate nums to the right by k steps in place; n >= 1, k >= 0 and k may exceed n.


# Straightforward: element i moves to (i + k) % n in a copy. T: O(n), S: O(n).
def rotate_copy(nums, k):
    n = len(nums)
    # result[(i + k) % n] = nums[i]
    result = [0] * n
    for i in range(n):
        result[(i + k) % n] = nums[i]
    # slice assignment writes into the caller's list; nums = result would not
    nums[:] = result


# Reverse nums[left..right] in place; a slice like nums[:k][::-1] would copy.
def reverse_range(nums, left, right):
    while left < right:
        nums[left], nums[right] = nums[right], nums[left]
        left += 1
        right -= 1


# Improved: reverse all, then reverse the first k and the rest. T: O(n), S: O(1).
def rotate(nums, k):
    n = len(nums)
    # rotating by n is a no-op
    k %= n
    reverse_range(nums, 0, n - 1)
    reverse_range(nums, 0, k - 1)
    reverse_range(nums, k, n - 1)


def test(label, nums, k, want):
    copied = list(nums)
    reversed_in_place = list(nums)
    rotate_copy(copied, k)
    rotate(reversed_in_place, k)
    ok = copied == want and reversed_in_place == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("basic", [1, 2, 3, 4], 1, [4, 1, 2, 3])
    test("big-k", [1, 2, 3], 4, [3, 1, 2])
    test("zero", [1, 2], 0, [1, 2])


main()

# follow-up: rotate left by k -> rotate right by n - k % n, or reverse the first k,
#   the rest, then all
