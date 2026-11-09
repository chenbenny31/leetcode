# LC 88 Merge Sorted Array (Easy). Pattern: two pointers filling from the back.
# nums1 holds m sorted values followed by n free slots; merge the n sorted values of
# nums2 into nums1 in place, sorted ascending.


# Fill nums1 from the back with the larger tail value, so nothing unread is
# overwritten. T: O(m + n), S: O(1).
def merge(nums1, m, nums2, n):
    # last unread value in nums1 / nums2
    read1 = m - 1
    read2 = n - 1
    write = m + n - 1
    # stop when nums2 is used up: the rest of nums1 is already in place
    while read2 >= 0:
        if read1 >= 0 and nums1[read1] > nums2[read2]:
            nums1[write] = nums1[read1]
            read1 -= 1
        else:
            nums1[write] = nums2[read2]
            read2 -= 1
        write -= 1


def test(label, nums1, m, nums2, want):
    merge(nums1, m, nums2, len(nums2))
    print(label, "PASS" if nums1 == want else "FAIL")


def main():
    test("interleave", [1, 4, 0, 0], 2, [2, 3], [1, 2, 3, 4])
    test("all-smaller", [5, 6, 0], 2, [1], [1, 5, 6])
    test("empty-first", [0, 0], 0, [1, 2], [1, 2])
    test("empty-second", [3], 1, [], [3])


main()

# follow-up: why from the back -> the free slots are at the back; filling from the
#   front would overwrite unread nums1 values
# follow-up: descending inputs -> place the smaller tail value at the back instead
