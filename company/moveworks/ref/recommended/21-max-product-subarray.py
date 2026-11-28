# 21 · Maximum Product Subarray (ServiceNow NLP intern 2024, Medium) — LC 152
# return the largest product of a contiguous non-empty subarray of nums
#
# clarify:
# negatives and zeros -> yes
# empty nums -> not given
# product size -> fits in a 32-bit int on LC; Python ints never overflow anyway


# core idea: track the max and the min product ending here; a negative num swaps them
# T: O(n), S: O(1)
def max_product(nums):
    # hi / lo = largest / smallest product of a subarray ending at the cur idx
    hi = nums[0]
    lo = nums[0]
    best = nums[0]
    for x in nums[1:]:
        if x < 0:
            hi, lo = lo, hi
        hi = max(x, hi * x)
        lo = min(x, lo * x)
        best = max(best, hi)
    return best


def test(label, nums, want):
    ok = max_product(nums) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("two-negatives", [-2, 3, -4], 24)
    test("single-negative", [-3], -3)
    test("zero-resets", [-1, 0, -2], 0)
    test("zero-split", [2, 0, 3, 4], 12)


main()

# follow-up:
# 1. why keep the min -> a negative times the most negative becomes the max
# 2. max subarray sum (LC 53) -> same scan, only the max: cur = max(x, cur + x)
# 3. max sum with one deletion (LC 1186) -> two DPs: no deletion / one deletion so far
