from collections import Counter

# keep the strs with min prop, drop the chars other strs have, concat

# return the prop of max-freq cnt / total-cnt
def proportion(s):
    counter = Counter(s)
    top = max(counter.values())
    total = 0
    for cnt in counter.values():
        if cnt == top:
            total += cnt
    return total / len(s)

# keep strs without chars occur in any other, then concat them
def drop_shared(words, keep):
    in_words = Counter()
    for w in words:
        for c in set(w):
            in_words[c] += 1
    out = ""
    for i in keep:
        for c in words[i]:
            if in_words[c] == 1:
                out += c
    return out

# return idxs of strs with min prop: compute all, find min, collect
# T: O(total-len), S: O(n)
def lowest_idxs(words):
    prop = [proportion(w) for w in words]
    lowest = min(prop)
    return [i for i in range(len(words)) if prop[i] == lowest]

def lowest_proportion(words):
    return drop_shared(words, lowest_idxs(words))

# keep strs whose min sim to others is lowest

# return sum of min counts / sum of max counts
def sim(a, b):
    cnt_a = Counter(a)
    cnt_b = Counter(b)
    share = 0
    total = 0
    for c in set(a) | set(b):
        share += min(cnt_a[c], cnt_b[c])
        total += max(cnt_a[c], cnt_b[c])
    if total == 0:
        return 0.0
    return share / total

# use score over prop
def lowest_proportion_by_sim(words):
    cand = lowest_idxs(words)
    k = len(cand)
    if k == 1:
        return drop_shared(words, cand)
    score = [1.0] * k  # score[a] = min sim between a and other cands
    for a in range(k):
        for b in range(k):
            if a == b:
                continue
            score[a] = min(score[a], sim(words[cand[a]], words[cand[b]]))
    lowest = min(score)
    keep = [cand[a] for a in range(k) if score[a] == lowest]
    return drop_shared(words, keep)

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
