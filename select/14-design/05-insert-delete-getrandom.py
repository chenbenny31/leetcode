# LC 380 Insert Delete GetRandom O(1) (Medium). Pattern: list + index map, swap-pop.
# insert(val) / remove(val) return whether the set changed; get_random() returns a
# uniformly random current element (called only when non-empty). All O(1) average.

import random


# Dense list for random access; remove moves the last element into the hole.
# T: O(1) average per op, S: O(n).
class RandomizedSet:
    def __init__(self):
        # the elements, in arbitrary order
        self.vals = []
        # val -> its position in vals
        self.index = {}

    def insert(self, val):
        if val in self.index:
            return False
        self.index[val] = len(self.vals)
        self.vals.append(val)
        return True

    def remove(self, val):
        if val not in self.index:
            return False
        pos = self.index[val]
        last = self.vals[-1]
        self.vals[pos] = last
        # before del: val may be the last element itself
        self.index[last] = pos
        self.vals.pop()
        del self.index[val]
        return True

    def get_random(self):
        return self.vals[random.randrange(len(self.vals))]


def test(label, got, want):
    print(label, "PASS" if got == want else "FAIL")


def main():
    rand_set = RandomizedSet()
    rand_set.insert(1)
    rand_set.insert(2)
    test("dup-insert", rand_set.insert(1), False)
    test("remove-missing", rand_set.remove(3), False)
    pick = rand_set.get_random()
    test("random-member", pick == 1 or pick == 2, True)
    test("remove-first", rand_set.remove(1), True)
    test("one-left", rand_set.get_random(), 2)
    test("remove-last", rand_set.remove(2), True)


main()

# follow-up: duplicates allowed (LC 381) -> index: val -> set of positions; remove takes
#   any position, moves vals[-1] there, fixes the moved value's set
# follow-up: randomness -> random is already a Mersenne Twister (C++'s rand() % n is the
#   weak one); random.choice(self.vals) is the same pick
