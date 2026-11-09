# LC 347 Top K Frequent Elements (Medium). Pattern: count + size-k min-heap / buckets.
# Return the k most frequent values of nums, in any order. The answer is unique.

import heapq
from collections import Counter


# Count, then keep the k most frequent (count, value) in a min-heap.
# T: O(n log k), S: O(n).
def top_k_frequent(nums, k):
    # freq[value] = occurrences in nums
    freq = Counter(nums)
    # (count, value) of the k most frequent so far, least frequent on top
    heap = []
    for val, count in freq.items():
        heapq.heappush(heap, (count, val))
        if len(heap) > k:
            heapq.heappop(heap)
    result = []
    while heap:
        count, val = heapq.heappop(heap)
        result.append(val)
    return result


# Bucket sort: a count is at most n, so scan buckets from high to low.
# T: O(n), S: O(n).
def top_k_frequent_bucket(nums, k):
    # freq[value] = occurrences in nums
    freq = Counter(nums)
    n = len(nums)
    # buckets[count] = values occurring exactly count times
    buckets = [[] for _ in range(n + 1)]
    for val, count in freq.items():
        buckets[count].append(val)
    result = []
    for count in range(n, 0, -1):
        for val in buckets[count]:
            result.append(val)
            if len(result) == k:
                return result
    return result


def test(label, nums, k, want):
    # any order is accepted: compare sorted
    ok = (sorted(top_k_frequent(nums, k)) == sorted(want)
          and sorted(top_k_frequent_bucket(nums, k)) == sorted(want))
    print(label, "PASS" if ok else "FAIL")


def main():
    test("top-two", [4, 4, 4, 6, 6, 9], 2, [4, 6])
    test("all", [3, 8], 2, [3, 8])
    test("negative", [-2, -2, 5], 1, [-2])


main()

# follow-up: k-th most frequent words in lexicographic order (LC 692) -> heap of
#   (-count, word); a size-k min-heap needs a wrapper class whose __lt__ flips the word
# follow-up: stream -> keep freq map + heap; or quickselect on (count, value), O(n) avg
