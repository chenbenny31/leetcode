# LC 341 Flatten Nested List Iterator (Medium). Pattern: stack of (list, index).
# Iterate a nested list of integers in order: next() returns the next integer,
# has_next() says whether one is left. Lists may be empty or deeply nested.


# minimal stand-in for LC's NestedInteger interface
class NestedInteger:
    def __init__(self, is_int, val, items):
        self.is_int = is_int
        self.val = val
        self.items = items

    def is_integer(self):
        return self.is_int

    def get_integer(self):
        return self.val

    def get_list(self):
        return self.items


# Lazy DFS: has_next descends into lists and pops finished ones until an integer is on
# top. T: O(1) amortized per call, S: O(depth).
class NestedIterator:
    def __init__(self, nested_list):
        # [list, next index in it] (a list so the index can be bumped in place);
        # top = innermost list being walked
        self.stack = [[nested_list, 0]]

    def next(self):
        # makes sure an integer is on top
        self.has_next()
        top = self.stack[-1]
        val = top[0][top[1]].get_integer()
        top[1] += 1
        return val

    def has_next(self):
        while self.stack:
            top = self.stack[-1]
            items = top[0]
            index = top[1]
            if index == len(items):
                self.stack.pop()
                continue
            cur = items[index]
            if cur.is_integer():
                return True
            # step past the sublist first so the parent resumes after it
            top[1] = index + 1
            self.stack.append([cur.get_list(), 0])
        return False


def num(value):
    return NestedInteger(True, value, [])


def nest(items):
    return NestedInteger(False, 0, items)


def test(label, nested_list, want):
    it = NestedIterator(nested_list)
    got = []
    while it.has_next():
        got.append(it.next())
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("mixed", [num(1), nest([num(2), num(3)]), num(4)], [1, 2, 3, 4])
    test("deep", [nest([nest([nest([num(5)])])]), num(6)], [5, 6])
    test("empty-lists", [nest([]), nest([nest([])])], [])


main()

# follow-up: simplest -> dfs-flatten everything into a list in the constructor,
#   then walk an index; O(total) memory up front instead of O(depth)
# follow-up: Pythonic -> a recursive generator (yield from) gives the same lazy walk,
#   but has_next then needs a one-item lookahead buffer
