# LC 11 Container With Most Water (Medium). Pattern: two pointers, move shorter side.
# Pick two lines height[i], height[j] (i < j); maximize (j - i) * min(height[i],
# height[j]).


# Start widest; any narrower pair using the shorter line is capped by it, so it can
# never beat the current area: discard the shorter line. T: O(n), S: O(1).
def max_area(height):
    left = 0
    right = len(height) - 1
    best = 0
    while left < right:
        width = right - left
        best = max(best, width * min(height[left], height[right]))
        if height[left] < height[right]:
            left += 1
        else:
            right -= 1
    return best


def test(label, height, want):
    got = max_area(height)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("tall-ends", [3, 1, 1, 3], 9)
    test("inner-pair", [1, 5, 4, 3], 6)
    test("two-lines", [2, 2], 2)


main()

# follow-up: why moving the taller side is useless -> width shrinks and min() is still
#   capped by the shorter line, so the area can only drop
# follow-up: trapped water between all bars (LC 42) -> same pointers, sum per bar
