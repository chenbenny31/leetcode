# LC 560 Subarray Sum Equals K (Medium). Pattern: prefix sum + hash map count.
# Count the contiguous non-empty subarrays of nums whose sum equals k; values may be
# negative, so a sliding window does not work.


# A subarray ending here sums to k iff an earlier prefix equals prefix - k.
# T: O(n), S: O(n).
def subarray_sum(nums, k):
    # prefix_count[p] = earlier prefixes with sum p; the empty prefix counts once
    prefix_count = {0: 1}
    prefix = 0
    count = 0
    for num in nums:
        prefix += num
        if prefix - k in prefix_count:
            count += prefix_count[prefix - k]
        # after the lookup, so k == 0 never counts an empty subarray
        prefix_count[prefix] = prefix_count.get(prefix, 0) + 1
    return count


def test(label, nums, k, want):
    got = subarray_sum(nums, k)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("overlap", [1, 2, 1], 3, 2)
    test("negative-zero-k", [1, -1, 0], 0, 3)
    test("none", [1, 2], 5, 0)


main()

# follow-up: all positive -> sliding window, shrink while sum > k, count when == k
# follow-up: longest subarray with sum k (LC 325) -> store first index of each prefix
#   only, best = max(i - first[prefix - k]), O(n)
# follow-up: sum divisible by k (LC 974) -> key prefix % k (Python's % is already
#   non-negative for k > 0; C++ needs ((prefix % k) + k) % k), same counting
