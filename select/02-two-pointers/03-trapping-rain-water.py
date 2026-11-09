# LC 42 Trapping Rain Water (Hard). Pattern: two pointers with running side maxes.
# height[i] = elevation of a width-1 bar; return how much rain water is trapped
# between the bars.


# Water at i = min(tallest left, tallest right) - height[i]; the lower side's max
# is that min, so settle the lower side and move it inward. T: O(n), S: O(1).
def trap(height):
    left = 0
    right = len(height) - 1
    # tallest bar in height[0..left]
    left_max = 0
    # tallest bar in height[right..n-1]
    right_max = 0
    water = 0
    while left < right:
        # the shorter side is the limit: the other side has a wall at least as tall
        if height[left] < height[right]:
            left_max = max(left_max, height[left])
            water += left_max - height[left]
            left += 1
        else:
            right_max = max(right_max, height[right])
            water += right_max - height[right]
            right -= 1
    return water


def test(label, height, want):
    got = trap(height)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("one-basin", [2, 0, 2], 2)
    test("uneven-walls", [3, 0, 1, 0, 2], 5)
    test("no-basin", [1, 2, 3], 0)
    test("empty", [], 0)


main()

# follow-up: easiest to derive -> left_max[i], right_max[i] lists first;
#   water += min(left_max[i], right_max[i]) - height[i], O(n) space
# follow-up: monotonic stack -> decreasing stack of indices; a taller bar pops bottom,
#   water += (min(height[left], height[i]) - height[bottom]) * (i - left - 1)
# follow-up: 2D (LC 407) -> heapq of boundary cells, flood inward, O(mn log mn)
