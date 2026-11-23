# 03 · Shortest Uncommon Substring (Moveworks phone screen, Medium) — LC 3076
# (1) LC 3076: for each str, return its shortest substring that occurs in no other
#     str; "" if it has none
# (2) 2026 phone screen: wrap that substring in <u></u> inside the original str;
#     compare case-insensitively; a tie goes to the earliest start
#
# clarify:
# several shortest ones -> (1) lexicographically smallest, (2) earliest start
# (2) no unique substring -> return the str unchanged (assumption)
# two equal strs -> each substring occurs in the other, so both get ""
# sizes -> n <= 100 strs, len <= 20, so trying every substring is fine


# ==================== (1) ====================
# shortest substring of each str found in no other str; tie -> smallest; none -> ""

# does sub occur in any str other than arr[skip]
# T: O(n * L^2), S: O(1)
def in_others(arr, skip, sub):
    for j in range(len(arr)):
        if j != skip and sub in arr[j]:
            return True
    return False


# core idea: for each str try substrings shortest first; at the first len that
# has a unique one, keep the smallest of that len
# T: O(n * L^2 * n * L^2) — about 1e8 char compares at the limits, fine
# S: O(L)
def shortest_substrings(arr):
    out = []
    for k in range(len(arr)):
        s = arr[k]
        best = ""
        for size in range(1, len(s) + 1):
            for i in range(len(s) - size + 1):
                sub = s[i:i + size]
                if in_others(arr, k, sub):
                    continue
                if best == "" or sub < best:
                    best = sub
            # found one at this len, so no longer len can win
            if best != "":
                break
        out.append(best)
    return out


# ==================== (2) ====================
# wrap that substring in <u></u>, case-insensitive; tie -> earliest start

# one str of (2): the first unique substring (shortest, then leftmost) gets the tags
# T: O(L^2 * n * L^2), S: O(L)
def underline_one(lower, k, original):
    n = len(original)
    for size in range(1, n + 1):
        for i in range(n - size + 1):
            if in_others(lower, k, lower[k][i:i + size]):
                continue
            # cut the original, so its case is kept
            before = original[:i]
            middle = original[i:i + size]
            after = original[i + size:]
            return before + "<u>" + middle + "</u>" + after
    return original


# core idea: search on lowercase copies, tag the original str
# T: same as (1), S: O(n * L)
def underline_unique(arr):
    # lower[k] = arr[k] in lowercase, used only for the search
    lower = [s.lower() for s in arr]
    out = []
    for k in range(len(arr)):
        out.append(underline_one(lower, k, arr[k]))
    return out


# ==================== tests ====================

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

# follow-up:
# 1. faster -> dict substring -> num of strs containing it; unique = count 1
# 2. longer strs -> rolling hash of substrings, or a generalized suffix automaton
# 3. only one query str -> build the count dict once, reuse it
