# LC 269 Alien Dictionary (Hard). Pattern: build edges + Kahn's topological sort.
# words are sorted by an unknown alien alphabet. Return any valid order of the letters
# that appear, or "" if none is consistent (cycle, or a word before its own prefix).

from collections import deque


# First differing letter of each adjacent pair gives an edge; then Kahn's BFS.
# T: O(C + V + E), S: O(V + E); C = total characters, V <= 26.
def alien_order(words):
    # graph[c] = letters that must come after c (set dedupes edges)
    graph = [set() for _ in range(26)]
    # indeg[c] = distinct letters that must come before c
    indeg = [0] * 26
    # present[c] = c appears in some word
    present = [False] * 26
    for word in words:
        for ch in word:
            present[ord(ch) - ord("a")] = True
    for i in range(len(words) - 1):
        first = words[i]
        second = words[i + 1]
        common = min(len(first), len(second))
        pos = 0
        while pos < common and first[pos] == second[pos]:
            pos += 1
        if pos == common:
            # second is a proper prefix of first yet comes after it: impossible
            if len(first) > len(second):
                return ""
            continue
        before = ord(first[pos]) - ord("a")
        after = ord(second[pos]) - ord("a")
        # count each edge once, or indeg[after] never reaches 0
        if after in graph[before]:
            continue
        graph[before].add(after)
        indeg[after] += 1
    # present letters with no unplaced predecessor
    q = deque()
    letter_count = 0
    for c in range(26):
        if not present[c]:
            continue
        letter_count += 1
        if indeg[c] == 0:
            q.append(c)
    # letters in the order placed
    order = []
    while q:
        cur = q.popleft()
        order.append(chr(ord("a") + cur))
        for nxt in graph[cur]:
            indeg[nxt] -= 1
            if indeg[nxt] == 0:
                q.append(nxt)
    # letters left out sit on a cycle
    if len(order) != letter_count:
        return ""
    return "".join(order)


def test(label, words, want):
    print(label, "PASS" if alien_order(words) == want else "FAIL")


def main():
    test("chain", ["ba", "bc", "ac", "cab"], "bac")
    test("dup-edge", ["aa", "ab", "ba"], "ab")
    test("cycle", ["a", "b", "a"], "")
    test("bad-prefix", ["abc", "ab"], "")


main()

# follow-up: lexicographically smallest order -> heapq min-heap instead of the deque,
#   O(C + 26 log 26)
# follow-up: is the order unique? -> queue size > 1 at any pop means not unique
