# LC 238 Product of Array Except Self (Medium). Pattern: prefix * suffix products.
# answer[i] = product of all nums except nums[i], without division, in O(n); the output
# array does not count as extra space.


# Left products into answer, then multiply in a running right product.
# No division, so zeros need no special case. T: O(n), S: O(1) extra.
def product_except_self(nums):
    n = len(nums)
    # after pass 1: answer[i] = product of nums[0..i-1]
    answer = [1] * n
    for i in range(1, n):
        answer[i] = answer[i - 1] * nums[i - 1]
    # product of nums[i+1..n-1]
    right = 1
    for i in range(n - 1, -1, -1):
        answer[i] *= right
        right *= nums[i]
    return answer


def test(label, nums, want):
    got = product_except_self(nums)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("basic", [2, 3, 4], [12, 8, 6])
    test("one-zero", [2, 0, 3], [0, 6, 0])
    test("two-zeros", [0, 2, 0], [0, 0, 0])
    test("negative", [-1, 2], [2, -1])


main()

# follow-up: division allowed -> total // nums[i], but zeros need cases (0 / 1 / 2+ zeros)
# follow-up: clearer O(n) space -> prefix[i] and suffix[i] lists,
#   answer[i] = prefix[i] * suffix[i]
