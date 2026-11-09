# LC 128 Longest Consecutive Sequence (Medium). Pattern: hash set, count from run heads.
# Return the length of the longest run of consecutive integers (x, x+1, ...) whose
# values all appear in unsorted nums, in O(n).


# Count up only from num with no num - 1 (the head of a run). T: O(n), S: O(n).
def longest_consecutive(nums):
    # distinct values of nums
    present = set(nums)
    best = 0
    for num in present:
        # non-heads skip, so each run is walked once in total
        if num - 1 in present:
            continue
        length = 1
        while num + length in present:
            length += 1
        best = max(best, length)
    return best


def test(label, nums, want):
    got = longest_consecutive(nums)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("two-runs", [10, 3, 11, 1, 2, 12, 13], 4)
    test("dups", [2, 1, 2, 3], 3)
    test("negatives", [-1, 1, 0], 3)
    test("empty", [], 0)


main()

# follow-up: why O(n) -> the while loop runs only from heads, so all inner steps sum to n
# follow-up: O(1) extra space allowed to modify -> nums.sort() + scan, skip equal
#   neighbors, O(n log n)
