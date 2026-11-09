# LC 767 Reorganize String (Medium). Pattern: max-heap greedy by remaining count.
# Rearrange s (lowercase) so no two adjacent characters are equal; return any such
# string, or "" if impossible.

import heapq


# Always place the most frequent remaining char that differs from the last one: pop the
# top two, write both, push back whatever is left. T: O(n log 26), S: O(26).
def reorganize_string(s):
    # count[letter] = copies of that letter in s
    count = [0] * 26
    for c in s:
        count[ord(c) - ord("a")] += 1
    # (-remaining count, letter), most remaining on top
    heap = []
    for i in range(26):
        if count[i] > 0:
            heapq.heappush(heap, (-count[i], chr(ord("a") + i)))

    # chars written so far; joined once at the end
    result = []
    while len(heap) >= 2:
        neg_a, letter_a = heapq.heappop(heap)
        count_a = -neg_a
        neg_b, letter_b = heapq.heappop(heap)
        count_b = -neg_b
        # last round's letter_b never outranks last round's letter_a once both lose one,
        # so this round's letter_a differs from the last char written
        result.append(letter_a)
        result.append(letter_b)
        if count_a > 1:
            heapq.heappush(heap, (-(count_a - 1), letter_a))
        if count_b > 1:
            heapq.heappush(heap, (-(count_b - 1), letter_b))
    if heap:
        # one letter left: only one copy may remain, or it would sit next to itself
        if -heap[0][0] > 1:
            return ""
        result.append(heap[0][1])
    return "".join(result)


# valid = same letters as s and no equal neighbors
def valid(s, got):
    if len(got) != len(s):
        return False
    count = [0] * 26
    for c in s:
        count[ord(c) - ord("a")] += 1
    for c in got:
        count[ord(c) - ord("a")] -= 1
    for diff in count:
        if diff != 0:
            return False
    for i in range(1, len(got)):
        if got[i] == got[i - 1]:
            return False
    return True


def test(label, s, possible):
    got = reorganize_string(s)
    if possible:
        ok = valid(s, got)
    else:
        ok = got == ""
    print(label, "PASS" if ok else "FAIL")


def main():
    test("two-letters", "aab", True)
    test("impossible", "aaab", False)
    test("many-letters", "aaabbc", True)
    test("single", "z", True)


main()

# follow-up: why pop two at a time -> the second pick guarantees the next char differs
#   from the one just written without remembering it
# follow-up: quick feasibility -> possible iff max_count <= (n + 1) // 2
# follow-up: same letters at least k apart (LC 358) -> heap + a cooldown queue of k
