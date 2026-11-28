# 17 · Subarray Sum Equals K (ServiceNow, experienced hires, Medium) — LC 560
# count contiguous non-empty subarrays of nums with sum = k
#
# clarify:
# negatives / zeros -> yes, so no sliding window
# k = 0 or k < 0 -> ok, same code
# overlapping subarrays -> each counts
# empty nums -> 0


# core idea: sum(nums[i:j]) = prefix(j) - prefix(i); count earlier prefixes = prefix - k
# T: O(n), S: O(n)
def subarray_sum(nums, k):
    # prefix_cnt[p] = num of earlier prefixes with sum p; the empty prefix counts once
    prefix_cnt = {0: 1}
    prefix = 0
    count = 0
    for num in nums:
        prefix += num
        count += prefix_cnt.get(prefix - k, 0)
        # add after the lookup, so k = 0 never counts an empty subarray
        prefix_cnt[prefix] = prefix_cnt.get(prefix, 0) + 1
    return count


def test(label, nums, k, want):
    ok = subarray_sum(nums, k) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("overlap", [1, 2, 1], 3, 2)
    test("negative-zero-k", [1, -1, 0], 0, 3)
    test("negative-k", [-1, 1, -1, -1], -1, 5)
    test("none", [1, 2], 5, 0)


main()

# follow-up:
# 1. all values positive -> sliding window, O(1) space
# 2. longest subarray with sum k (LC 325) -> keep the FIRST idx of each prefix
# 3. equal 0s and 1s (LC 525) -> 0 counts as -1, longest with sum 0
# 4. sum divisible by k (LC 974) -> key the dict by prefix % k (Python % is never < 0)
