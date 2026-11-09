# LC 875 Koko Eating Bananas (Medium). Pattern: binary search on the answer.
# piles[i] bananas; each hour Koko eats up to k bananas from one pile (a smaller pile
# still takes the whole hour). Return the minimum k that finishes within h hours.


# Hours needed at speed k (Python ints don't overflow, no long long needed).
# T: O(n), S: O(1).
def hours_needed(piles, k):
    hours = 0
    for pile in piles:
        # ceil(pile / k) for pile >= 1, in integers (no float rounding)
        hours += (pile - 1) // k + 1
    return hours


# Lower-bound template on k in [1, max pile]: feasible (hours <= h) is monotonic in k,
# so find the first feasible k. T: O(n log(max pile)), S: O(1).
def min_eating_speed(piles, h):
    lo = 1
    hi = max(piles)
    while lo < hi:
        mid = (lo + hi) // 2
        if hours_needed(piles, mid) <= h:
            hi = mid
        else:
            lo = mid + 1
    return lo


def test(label, piles, h, want):
    got = min_eating_speed(piles, h)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("tight", [4, 9], 3, 5)
    test("one-hour-per-pile", [4, 9], 2, 9)
    test("plenty-of-time", [4, 9], 100, 1)
    test("single-pile", [7], 3, 3)


main()

# follow-up: recognize it -> "minimize X such that check(X) passes" + check is
#   monotonic in X; same shape: ship packages in D days (LC 1011), LC 410
