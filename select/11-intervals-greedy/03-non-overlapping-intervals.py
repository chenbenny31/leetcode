# LC 435 Non-overlapping Intervals (Medium). Pattern: greedy, sort by end, keep earliest.
# Return the min number of intervals to remove so the rest are pairwise
# non-overlapping; touching at an endpoint is fine.


# Sort by end; keep every interval that starts at or after the last kept end.
# T: O(n log n), S: O(n) for the sorted copy.
def erase_overlap_intervals(intervals):
    intervals = sorted(intervals, key=lambda interval: interval[1])
    n = len(intervals)
    kept = 0
    # end of the last kept interval; -inf so the first interval is always kept
    last_end = float("-inf")
    for cur in intervals:
        # touching (start == last_end) is not an overlap
        if cur[0] >= last_end:
            kept += 1
            last_end = cur[1]
    return n - kept


def test(label, intervals, want):
    print(label, "PASS" if erase_overlap_intervals(intervals) == want else "FAIL")


def main():
    test("touching", [[1, 3], [3, 5], [5, 7]], 0)
    test("nested", [[1, 10], [2, 3], [4, 5]], 1)
    test("dups", [[0, 2], [0, 2]], 1)
    test("negative", [[-3, -1], [-2, 0], [-1, 1]], 1)


main()

# follow-up: why the greedy is optimal -> exchange: the earliest-ending interval can
#   replace the first one of any optimal set without new overlaps; repeat on the rest
# follow-up: min arrows (LC 452) -> same sort; touching counts as overlap, so a new
#   arrow only when start > last_end
