# 05 · Word Break (Moveworks onsite 2026-09, Medium) — LC 139
# return if s splits into a sequence of words from word_dict; words reusable
#
# clarify:
# words reusable -> no used-tracking
# every char of s must be covered -> answer = dp[n]
# case-sensitive -> compare as is
# empty s -> True
# sizes of s / dict -> huge dict: set + max word len (follow-up)


# ==================== solution 1: recursion + memo (start here) ====================
# same shape as Word Break II: cut a dict word off the front, ask if the rest splits

# can s[start:] be split; memo[start] caches the answer for each suffix
def can_split(s, start, words, memo):
    if start == len(s):
        return True
    if start in memo:
        return memo[start]
    for end in range(start + 1, len(s) + 1):
        if s[start:end] in words and can_split(s, end, words, memo):
            memo[start] = True
            return True
    memo[start] = False
    return False


# T: O(n^3) — n suffixes, n cuts each, O(n) slice; S: O(n + dict)
def word_break_memo(s, word_dict):
    # dictionary words for O(1) lookup
    words = set(word_dict)
    memo = {}
    return can_split(s, 0, words, memo)


# ==================== solution 2: bottom-up DP ====================
# same idea filled left to right: s[:i] splits if a word ends at i and the prefix
# before that word splits

# T: O(n * W * L), S: O(n) — W words of len <= L
def word_break(s, word_dict):
    n = len(s)
    # dp[i] = s[:i] can be split into dict words
    dp = [False] * (n + 1)
    dp[0] = True
    for i in range(1, n + 1):
        for word in word_dict:
            # word would occupy s[start:i]
            start = i - len(word)
            if start >= 0 and dp[start] and s[start:i] == word:
                dp[i] = True
                break
    return dp[n]


# ==================== tests ====================

def test(label, s, word_dict, want):
    ok = word_break(s, word_dict) == want and word_break_memo(s, word_dict) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("two-words", "catdog", ["cat", "dog"], True)
    test("greedy-trap", "abcd", ["a", "abc", "bcd"], True)
    test("reuse", "aaaaa", ["aa", "aaa"], True)
    test("no-split", "aaaa", ["aaa"], False)


main()

# follow-up:
# 1. return all sentences (LC 140) -> ref/optional/25-word-break-ii
# 2. huge dict -> try only s[j:i] for j in [i - max word len, i)
# 3. return one split -> keep cut[start] = the end that worked, follow it from 0
# 4. decode ways (LC 91) -> dp[i] += dp[i - 1] if s[i - 1] != '0', += dp[i - 2] if 10..26
