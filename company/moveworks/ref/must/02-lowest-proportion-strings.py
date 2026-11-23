# 02 · Lowest Proportion Strings (Moveworks HackerRank screen, Medium) — counting
# proportion(s) = (total count of all chars tied for the max freq in s) / len(s)
# (1) keep every str with the min proportion; from each, drop every char that occurs in
#     any other input str; concatenate the rest in input order
# (2) same, but among the kept strs c, score(c) = min sim(c, c') over the other kept
#     c'; keep only those with the min score (one kept str -> keep it) before dropping
# (3) implement sim: Jaccard similarity that counts repeated chars
#
# clarify:
# chars -> lowercase letters
# ties -> every str tied at the min proportion (or min score) is kept
# "other str" -> any other idx in the original list, kept or not
# (3) Jaccard with repeats -> sum of min counts / sum of max counts; both empty -> 0

from collections import Counter


# ==================== (1) ====================
# keep the strs with the min proportion, drop the chars other strs have, concat

# T: O(len), S: O(1)
def proportion(s):
    cnt = Counter(s)
    top = max(cnt.values())
    # every char tied at the top adds its count
    total = 0
    for f in cnt.values():
        if f == top:
            total += f
    return total / len(s)


# kept strs without the chars that occur in any other input str, concatenated
# T: O(total len), S: O(1) besides the output
def drop_shared(words, keep):
    # in_words[c] = num of input strs that contain c
    in_words = Counter()
    for w in words:
        for c in set(w):
            in_words[c] += 1
    out = ""
    for i in keep:
        for c in words[i]:
            # words[i] itself is one of them, so 1 means no other str has c
            if in_words[c] == 1:
                out += c
    return out


# idxs of the strs with the min proportion: compute all, find the min, collect
# T: O(total len), S: O(n)
def lowest_idxs(words):
    # prop[i] = proportion of words[i]
    prop = [proportion(w) for w in words]
    lowest = min(prop)
    return [i for i in range(len(words)) if prop[i] == lowest]


# T: O(total len), S: O(n)
def lowest_proportion(words):
    return drop_shared(words, lowest_idxs(words))


# ==================== (2) and (3) ====================
# among those strs keep the ones whose min sim to the others is lowest
# sim = Jaccard counting repeats: sum of min counts / sum of max counts

# used only by (2): per letter, shared = min of the two counts, total = max of them
# T: O(len a + len b), S: O(1)
def sim(a, b):
    cnt_a = Counter(a)
    cnt_b = Counter(b)
    shared = 0
    total = 0
    for c in set(a) | set(b):
        shared += min(cnt_a[c], cnt_b[c])
        total += max(cnt_a[c], cnt_b[c])
    if total == 0:
        return 0.0
    return shared / total


# same three steps as lowest_idxs, on scores instead of proportions
# T: O(total len + k^2 * L), S: O(k) — k = num of candidates, L = max len
def lowest_proportion_by_sim(words):
    cand = lowest_idxs(words)
    k = len(cand)
    if k == 1:
        return drop_shared(words, cand)
    # score[a] = min sim between candidate a and every other candidate
    score = [1.0] * k
    for a in range(k):
        for b in range(k):
            if a == b:
                continue
            score[a] = min(score[a], sim(words[cand[a]], words[cand[b]]))
    lowest = min(score)
    keep = [cand[a] for a in range(k) if score[a] == lowest]
    return drop_shared(words, keep)


# ==================== tests ====================

def test(label, part, words, want):
    if part == 1:
        got = lowest_proportion(words)
    else:
        got = lowest_proportion_by_sim(words)
    ok = got == want
    print(label, "PASS" if ok else "FAIL")


def test_sim(label, a, b, want):
    ok = abs(sim(a, b) - want) < 1e-9
    print(label, "PASS" if ok else "FAIL")


def main():
    test("p1-one-min", 1, ["aabbc", "aaaa", "xyzxyz"], "bbc")
    test("p1-all-tied", 1, ["abc", "def", "ghi"], "abcdefghi")
    test("p2-min-score", 2, ["abx", "by", "aby"], "x")
    test_sim("p3-repeats", "aab", "ab", 2 / 3)


main()

# follow-up:
# 1. == on floats safe? -> yes: equal fractions round to the same float
# 2. exact anyway -> Fraction(total, len(s)) from fractions, compare exactly
# 3. sim is expensive -> k^2 calls; sim(a, b) = sim(b, a), so compute each pair once
# 4. Jaccard on sets, repeats ignored -> len(set(a) & set(b)) / len(set(a) | set(b))
# 5. Counter shortcut -> sum((cnt_a & cnt_b).values()) / sum((cnt_a | cnt_b).values())
