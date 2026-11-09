# LC 739 Daily Temperatures (Medium). Pattern: monotonic decreasing stack.
# For each day, the number of days until a strictly warmer temperature; 0 if none.


# Stack of days still waiting; a warmer day resolves every colder day on top.
# T: O(n), S: O(n).
def daily_temperatures(temps):
    n = len(temps)
    # wait[i] = days until warmer than day i, 0 if never
    wait = [0] * n
    # unresolved days, temps non-increasing bottom to top
    stk = []
    for i in range(n):
        while stk and temps[stk[-1]] < temps[i]:
            colder_day = stk.pop()
            wait[colder_day] = i - colder_day
        stk.append(i)
    return wait


def test(label, temps, want):
    got = daily_temperatures(temps)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("mixed", [60, 50, 70, 55, 65], [2, 1, 0, 1, 0])
    test("decreasing", [90, 60, 30], [0, 0, 0])
    test("equal-not-warmer", [50, 50, 51], [2, 1, 0])


main()

# follow-up: circular next greater (LC 503) -> loop i over 2n with i % n, push only i < n
# follow-up: stock span (LC 901) -> stack of (price, span); pop and add spans while <=
