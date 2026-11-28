# 25 · Word Break II (Moveworks onsite 2026-09, Hard) — LC 140
# str s and a dict of words (reusable); return every split of s into dict words,
# sentences joined by single spaces, any order; [] if no split
#
# clarify:
# len of s -> up to 20 on LC; the output itself can be exponential
# dup dict words -> the set dedups
# output order -> any


# ==================== solution 1: backtracking (start here) ====================
# cut a dict word off the front, recurse on the rest; at the end of s, the words
# picked on the way form one sentence

# words picked so far are in path; reaching the end of s = one full sentence
def backtrack(s, start, words, path, out):
    if start == len(s):
        out.append(" ".join(path))
        return
    for end in range(start + 1, len(s) + 1):
        word = s[start:end]
        if word not in words:
            continue
        path.append(word)
        backtrack(s, end, words, path, out)
        path.pop()


# T: O(2^n * n) worst case (every cut works), S: O(n) besides the output
def word_break(s, word_dict):
    # dictionary words for O(1) lookup
    words = set(word_dict)
    out = []
    backtrack(s, 0, words, [], out)
    return out


# ==================== solution 2: memo (when asked to speed it up) ====================
# same cuts, but dfs(start) returns all sentences of s[start:] and is cached, so a
# suffix reached by several prefixes is solved once

# dfs(n) = [""]: one empty sentence, not "no sentence"
# T: O(n^2 + output size), S: O(n * output size) — memo; output can be exponential
def dfs(s, start, words, memo):
    if start == len(s):
        return [""]
    if start in memo:
        return memo[start]
    result = []
    for end in range(start + 1, len(s) + 1):
        word = s[start:end]
        if word not in words:
            continue
        # every sentence of the rest gets this word in front
        for tail in dfs(s, end, words, memo):
            if tail == "":
                result.append(word)
            else:
                result.append(word + " " + tail)
    memo[start] = result
    return result


def word_break_memo(s, word_dict):
    words = set(word_dict)
    # memo[start] = all sentences of s[start:]
    memo = {}
    return dfs(s, 0, words, memo)


# ==================== tests ====================

def test(label, s, word_dict, want):
    by_backtrack = sorted(word_break(s, word_dict))
    by_memo = sorted(word_break_memo(s, word_dict))
    ok = by_backtrack == sorted(want) and by_memo == sorted(want)
    print(label, "PASS" if ok else "FAIL")


def main():
    test("two-splits", "catsdog", ["cat", "cats", "sand", "dog", "s"],
         ["cats dog", "cat s dog"])
    test("reuse", "aaaa", ["a", "aa"],
         ["a a a a", "aa a a", "a aa a", "a a aa", "aa aa"])
    test("no-split", "abc", ["ab"], [])


main()

# follow-up:
# 1. only yes / no (LC 139) -> ref/must/05-word-break
# 2. count the sentences -> dp[i] += dp[j] for each dict word s[j:i]
# 3. many prefixes share a suffix -> solution 2, memo[start] = sentences of s[start:]
