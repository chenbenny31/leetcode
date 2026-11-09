# LC 56 Merge Intervals (Medium). Pattern: sort by start, extend the last interval.
# Merge all overlapping closed intervals [start, end]; touching ones like [1,2] and
# [2,3] merge too. Input is not sorted.


# Sort by start, then either extend the last merged interval or start a new one.
# T: O(n log n), S: O(n).
def merge(intervals):
    intervals = sorted(intervals)
    # disjoint intervals in start order
    merged = []
    for cur in intervals:
        # closed intervals: start == last end still overlaps
        if merged and cur[0] <= merged[-1][1]:
            # max: cur may sit entirely inside the last one
            merged[-1][1] = max(merged[-1][1], cur[1])
        else:
            # copy: extending merged[-1] must not change the caller's interval
            merged.append(list(cur))
    return merged


def test(label, intervals, want):
    print(label, "PASS" if merge(intervals) == want else "FAIL")


def main():
    test("unsorted", [[8, 10], [1, 3], [2, 6]], [[1, 6], [8, 10]])
    test("touching", [[1, 2], [2, 3]], [[1, 3]])
    test("contained", [[1, 10], [2, 3]], [[1, 10]])
    test("empty", [], [])


main()

# follow-up: insert into sorted disjoint intervals (LC 57) -> copy those ending before
#   new.start, absorb overlaps (min start / max end), copy the rest; O(n), no sort
# follow-up: open intervals [s, e) -> use cur[0] < merged[-1][1]
