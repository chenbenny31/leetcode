# LC 763 Partition Labels (Medium). Pattern: last occurrence + greedy cut.
# Split lowercase s into as many parts as possible so each letter appears in at most
# one part; return the part sizes in order.


# A part can close only once every letter in it has had its last occurrence.
# T: O(n), S: O(26).
def partition_labels(s):
    n = len(s)
    # last[c] = last index of letter c in s
    last = [0] * 26
    for i in range(n):
        last[ord(s[i]) - ord("a")] = i
    sizes = []
    # first index of the current part
    start = 0
    # farthest last occurrence among letters in the current part
    end = 0
    for i in range(n):
        end = max(end, last[ord(s[i]) - ord("a")])
        if i == end:
            sizes.append(end - start + 1)
            start = i + 1
    return sizes


def test(label, s, want):
    print(label, "PASS" if partition_labels(s) == want else "FAIL")


def main():
    test("two-parts", "ababcc", [4, 2])
    test("all-distinct", "abcd", [1, 1, 1, 1])
    test("one-part", "abca", [4])
    test("empty", "", [])


main()

# follow-up: return the parts themselves -> append s[start:end + 1]
# follow-up: same idea as merging [first, last] intervals of each letter (LC 56)
