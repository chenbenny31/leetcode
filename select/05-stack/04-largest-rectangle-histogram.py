# LC 84 Largest Rectangle in Histogram (Hard). Pattern: monotonic increasing stack.
# Bars of width 1 with the given heights; return the area of the largest rectangle.


# When a bar is popped, the current bar and the new top are its first lower bars on the
# right and left, so its widest rectangle is known. T: O(n), S: O(n).
def largest_rectangle_area(heights):
    # a copy with a sentinel 0 that flushes every bar left on the stack
    heights = heights + [0]
    n = len(heights)
    # indices with strictly increasing heights
    stk = []
    best = 0
    for i in range(n):
        while stk and heights[stk[-1]] >= heights[i]:
            height = heights[stk.pop()]
            # first lower bar on the left, -1 if none
            left = stk[-1] if stk else -1
            width = i - left - 1
            best = max(best, height * width)
        stk.append(i)
    return best


def test(label, heights, want):
    got = largest_rectangle_area(heights)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("plateau", [1, 3, 3, 1], 6)
    test("increasing", [1, 2, 3], 4)
    test("low-and-wide", [2, 1, 2, 1, 2], 5)
    test("single", [5], 5)


main()

# follow-up: maximal rectangle in a 0/1 matrix (LC 85) -> per row, heights[c] += 1 on '1'
#   else reset to 0, run this on each row, O(m * n)
