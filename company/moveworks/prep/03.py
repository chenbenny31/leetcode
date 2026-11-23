# shortest substr of each str not found in any other, tie -> smallest, none -> ""

def in_others(arr, skip, sub):
    for i in range(len(arr)):
        if i != skip and sub in arr[i]:
            return True
    return False

# for each str try substr shortest first, keep uniq at first len
# T: O(nL^2nL^2), S: O(L)
def shortest_substrings(arr):
    out = []
    for k in range(len(arr)):
        s = arr[k]
        best = ""
        for size in range(1, len(s) + 1):
            for i in range(len(s) - size + 1):
                sub = s[i:i+size]
                if in_others(arr, k, sub):
                    continue
                if best == "" or sub < best:
                    best = sub
            if best != "":
                break
        out.append(best)
    return out

# wrap that substr in <u></u>, case-insensitive, tie -> earliest start

# the first uniq substr (shortest then leftmost) gets the tags
# T: O(L^2nL^2), S: O(L)
def underline_one(lower, k, original):
    n = len(original)
    for size in range(1, n + 1):
        for i in range(n - size + 1):
            if in_others(lower, k, lower[k][i:i+size]):
                continue
            before = original[:i]
            middle = original[i:i+size]
            after = original[i+size:]
            return before + "<u>" + middle + "</u>" + after
    return original

# search on lowercase copies, tag the original str, S: O(nL)
def underline_unique(arr):
    lower = [s.lower() for s in arr]
    out = []
    for k in range(len(arr)):
        out.append(underline_one(lower, k, arr[k]))
    return out

def test(label, part, arr, want):
    if part == 1:
        got = shortest_substrings(arr)
    else:
        got = underline_unique(arr)
    ok = got == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("p1-smallest-tie", 1, ["cba", "a", "b", "c"], ["ba", "", "", ""])
    test("p1-duplicates", 1, ["ab", "ab"], ["", ""])
    test("p2-earliest-and-case", 2, ["Cba", "a", "b", "c"], ["<u>Cb</u>a", "a", "b", "c"])
    test("p2-case-insensitive", 2, ["AB", "ab", "abc"], ["AB", "ab", "ab<u>c</u>"])


main()
