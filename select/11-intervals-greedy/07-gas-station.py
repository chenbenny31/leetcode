# LC 134 Gas Station (Medium). Pattern: one-pass greedy, reset start on negative tank.
# Circular route: gas[i] at station i, cost[i] to drive to the next one. Return the
# start index that completes the loop (unique if it exists), else -1.


# If the tank goes negative at i, no start in [start, i] works, so restart at i + 1.
# A start exists iff the total net gas >= 0. T: O(n), S: O(1).
def can_complete_circuit(gas, cost):
    n = len(gas)
    # net gas over the whole loop
    total = 0
    # net gas since the current start
    tank = 0
    start = 0
    for i in range(n):
        total += gas[i] - cost[i]
        tank += gas[i] - cost[i]
        if tank < 0:
            start = i + 1
            tank = 0
    if total < 0:
        return -1
    return start


def test(label, gas, cost, want):
    print(label, "PASS" if can_complete_circuit(gas, cost) == want else "FAIL")


def main():
    test("start-last", [1, 1, 5], [2, 2, 1], 2)
    test("start-middle", [1, 4, 2], [3, 1, 2], 1)
    test("not-enough", [1, 2], [2, 2], -1)
    test("single", [5], [4], 0)


main()

# follow-up: why total >= 0 suffices -> the part before start has net < 0, so the part
#   from start to the end carries more than enough to cover it after wrapping
