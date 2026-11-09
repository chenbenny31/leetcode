# LC 1429 First Unique Number (Medium). Pattern: queue + count, lazy cleanup.
# FirstUnique(nums) then add(value); show_first_unique() returns the oldest value that
# has appeared exactly once so far, or -1. (Amazon "oldest one-time visitor" family.)

from collections import defaultdict
from collections import deque


# Every value enters the queue once, in arrival order; the front is dropped lazily once
# it is seen again. T: O(1) amortized per op, S: O(distinct values).
class FirstUnique:
    def __init__(self, nums):
        # values in first-arrival order; the front may be stale (count > 1)
        self.order = deque()
        # count[v] = times v has been added
        self.count = defaultdict(int)
        for num in nums:
            self.add(num)

    def add(self, value):
        self.count[value] += 1
        if self.count[value] == 1:
            self.order.append(value)

    def show_first_unique(self):
        # each value is popped at most once, so this is O(1) amortized
        while self.order and self.count[self.order[0]] > 1:
            self.order.popleft()
        if not self.order:
            return -1
        return self.order[0]


def check(ok, label):
    print(label, "PASS" if ok else "FAIL")


def main():
    first_unique = FirstUnique([2, 3, 5])
    check(first_unique.show_first_unique() == 2, "oldest-first")
    first_unique.add(2)
    check(first_unique.show_first_unique() == 3, "repeat-drops-out")
    first_unique.add(3)
    first_unique.add(5)
    check(first_unique.show_first_unique() == -1, "none-left")
    first_unique.add(7)
    check(first_unique.show_first_unique() == 7, "new-unique")


main()

# follow-up: strict O(1) per op -> a dict used as an ordered set of the uniques (dicts
#   keep insertion order): del the value when it repeats, first = next(iter(uniques))
# follow-up: "unique within the last k events" -> also expire from the front by time
