# LC 895 Maximum Frequency Stack (Hard). Pattern: frequency -> stack of values.
# push(val); pop() removes and returns the most frequent value; among equally frequent
# values, the one pushed most recently.

from collections import defaultdict


# One stack per frequency level: a value with count c sits in stacks 1..c, so the top of
# the highest stack is always the answer. T: O(1) per op, S: O(pushes).
class FreqStack:
    def __init__(self):
        # freq[val] = current count of val
        self.freq = defaultdict(int)
        # group[count] = values that reached that count, in push order (top = most recent)
        self.group = defaultdict(list)
        # highest count present
        self.max_freq = 0

    def push(self, val):
        self.freq[val] += 1
        count = self.freq[val]
        if count > self.max_freq:
            self.max_freq = count
        self.group[count].append(val)

    def pop(self):
        val = self.group[self.max_freq].pop()
        self.freq[val] -= 1
        # the level is empty: the next highest count is exactly one less
        if not self.group[self.max_freq]:
            self.max_freq -= 1
        return val


def test(label, pushes, want):
    stack = FreqStack()
    for val in pushes:
        stack.push(val)
    got = []
    for _ in range(len(want)):
        got.append(stack.pop())
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("most-frequent", [1, 2, 1], [1, 2, 1])
    test("tie-most-recent", [1, 2, 2, 1], [1, 2, 2, 1])
    test("plain-stack", [3, 4, 5], [5, 4, 3])


main()

# follow-up: why not a heap of (-freq, -push_time, val) -> works, O(log n) per op, but
#   stale entries pile up; the per-level stacks give O(1)
# follow-up: peek() -> self.group[self.max_freq][-1] without popping
