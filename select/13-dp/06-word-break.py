# LC 139 Word Break (Medium). Pattern: prefix DP over split points.
# Can s be split into a sequence of words from word_dict? Words may be reused.


# s[0..i) splits if some word ends at i and the prefix before it splits.
# T: O(n * W * L) for W words of length <= L, S: O(n).
def word_break(s, word_dict):
    n = len(s)
    # dp[i] = s[0..i) can be split into dictionary words
    dp = [False] * (n + 1)
    dp[0] = True
    for i in range(1, n + 1):
        for word in word_dict:
            # word would occupy s[start..i)
            start = i - len(word)
            if start >= 0 and dp[start] and s[start:i] == word:
                dp[i] = True
                break
    return dp[n]


def test(label, s, word_dict, want):
    got = word_break(s, word_dict)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("two-words", "applepen", ["apple", "pen"], True)
    test("greedy-trap", "abcd", ["a", "abc", "bcd"], True)
    test("reuse", "aaaaa", ["aa", "aaa"], True)
    test("no-split", "aaaa", ["aaa"], False)


main()

# follow-up: return all sentences (LC 140) -> dfs(start) memoized in a dict
#   start -> list of sentences; output itself can be exponential
# follow-up: huge dict -> set of words, try s[j:i] for j in [i - max_len, i),
#   O(n * max_len^2)
