# 04 · Parse Citations, Then Merge (Moveworks onsite 2025, Medium) — str.find parse
# (1) text like "see [3], also [4-10]" -> the valid citation ranges in order:
#     [3] -> (3, 3), [4-10] -> (4, 10); no regex
# (2) parse two texts and merge all their ranges into sorted, non-overlapping ranges
#
# clarify:
# valid inside the brackets -> "n" or "a-b", digits only, a <= b; anything else skipped
# spaces inside ("[ 3]") -> invalid (assumption; ask)
# unclosed '[' -> ignored
# merge touching ranges ([1-3] and [4]) -> yes, citation nums are ints (assumption)
# text -> ASCII (isdigit also accepts other Unicode digits)


# ==================== (1) ====================
# citations "[3]" / "[4-10]" in a text -> ranges; skip invalid; no regex

# "123" -> 123, -1 if empty or not all digits. T: O(len), S: O(1)
def parse_num(s):
    if not s.isdigit():
        return -1
    return int(s)


# core idea: find each '[' and the next ']', split the inside at the first '-'
# T: O(len), S: O(len)
def parse_citations(text):
    out = []
    pos = 0
    while True:
        open_idx = text.find("[", pos)
        if open_idx == -1:
            break
        close_idx = text.find("]", open_idx)
        # unclosed '[' -> nothing more to read
        if close_idx == -1:
            break
        inside = text[open_idx + 1:close_idx]
        if "-" in inside:
            lo_text, hi_text = inside.split("-", 1)
        else:
            lo_text = inside
            hi_text = inside
        lo = parse_num(lo_text)
        hi = parse_num(hi_text)
        if lo != -1 and hi != -1 and lo <= hi:
            out.append((lo, hi))
        pos = close_idx + 1
    return out


# ==================== (2) ====================
# parse two texts, merge all ranges (touching too) into sorted disjoint ranges

# core idea: parse both, sort by start, extend the last range while they touch
# T: O(len + r log r), S: O(r) — r = num of ranges
def merge_citations(a, b):
    ranges = parse_citations(a) + parse_citations(b)
    ranges.sort()
    # [lo, hi] lists, so the last one can be extended in place
    merged = []
    for lo, hi in ranges:
        # + 1: [1-3] and [4] touch, so they merge
        if merged and lo <= merged[-1][1] + 1:
            merged[-1][1] = max(merged[-1][1], hi)
        else:
            merged.append([lo, hi])
    return merged


# ==================== tests ====================

# part 1 reads texts[0]; part 2 merges texts[0] and texts[1]
def test(label, part, texts, want):
    if part == 1:
        got = parse_citations(texts[0])
    else:
        got = merge_citations(texts[0], texts[1])
    ok = got == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("p1-in-text-order", 1, ["cite [7] and [2-4]"], [(7, 7), (2, 4)])
    test("p1-invalid-skipped", 1, ["[a] [5-2] [ 7] [-3] [3"], [])
    test("p2-merge", 2, ["[1-3] [8]", "[2-5] [9-12]"], [[1, 5], [8, 12]])
    test("p2-touching", 2, ["[1-3]", "[4]"], [[1, 4]])


main()

# follow-up:
# 1. report why a citation is invalid -> return (range, error) pairs instead of skipping
# 2. list form "[3, 5, 7-9]" -> split the inside on ',' first, then the same per piece
# 3. only overlapping (not touching) ranges merge -> use lo <= merged[-1][1]
# 4. ranges arrive over time -> sorted list of starts + bisect, merge with neighbors
# 5. merge intervals mother problem -> select/11-intervals-greedy/01-merge-intervals
