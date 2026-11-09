# LC 295 Find Median from Data Stream (Hard). Pattern: two heaps, lower/upper half.
# add_num(num) adds a value from a stream; find_median() returns the median so far (mean
# of the two middle values when the count is even).

import heapq


# Two heaps split the values into a smaller and a larger half; the median sits on
# their tops. add_num T: O(log n), find_median T: O(1), S: O(n).
class MedianFinder:
    def __init__(self):
        # max-heap (negated values): smaller half, holds the extra value when odd
        self.low = []
        # min-heap: larger half
        self.high = []

    def add_num(self, num):
        # route through low so every value in low <= every value in high
        heapq.heappush(self.low, -num)
        top = -heapq.heappop(self.low)
        heapq.heappush(self.high, top)
        if len(self.high) > len(self.low):
            top = heapq.heappop(self.high)
            heapq.heappush(self.low, -top)

    def find_median(self):
        if len(self.low) > len(self.high):
            return -self.low[0]
        return (-self.low[0] + self.high[0]) / 2


def check(ok, label):
    print(label, "PASS" if ok else "FAIL")


def main():
    mf = MedianFinder()
    mf.add_num(5)
    check(abs(mf.find_median() - 5.0) < 1e-9, "one")
    mf.add_num(-2)
    check(abs(mf.find_median() - 1.5) < 1e-9, "even-negative")
    mf.add_num(9)
    check(abs(mf.find_median() - 5.0) < 1e-9, "odd")
    mf.add_num(5)
    check(abs(mf.find_median() - 5.0) < 1e-9, "dup-middle")


main()

# follow-up: all values in [0, 100] -> count array of 101 buckets, walk to the middle,
#   O(100) per query
# follow-up: sliding window median (LC 480) -> two heaps + lazy deletion (no multiset
#   in Python), or one sorted list + bisect, O(k) per insert/remove
