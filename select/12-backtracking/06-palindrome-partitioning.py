# LC 131 Palindrome Partitioning (Medium). Pattern: backtracking on palindromic prefixes.
# Return every way to split s into substrings that are all palindromes.


def is_palindrome(s, left, right):
    while left < right:
        if s[left] != s[right]:
            return False
        left += 1
        right -= 1
    return True


# path = pieces chosen for s[0..start)
def dfs(s, start, path, result):
    n = len(s)
    if start == n:
        # copy: path keeps changing after this
        result.append(list(path))
        return
    for end in range(start, n):
        # only a palindromic piece can lead to a valid split
        if not is_palindrome(s, start, end):
            continue
        path.append(s[start:end + 1])
        dfs(s, end + 1, path, result)
        path.pop()


# Choose the next palindromic prefix of the rest. T: O(n * 2^n), S: O(n) + output.
def partition(s):
    result = []
    path = []
    dfs(s, 0, path, result)
    return result


def test(label, s, want):
    got = partition(s)
    got.sort()
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("middle", "aba", [["a", "b", "a"], ["aba"]])
    test("pairs", "aabb", [["a", "a", "b", "b"], ["a", "a", "bb"],
                           ["aa", "b", "b"], ["aa", "bb"]])
    test("no-long", "ab", [["a", "b"]])


main()

# follow-up: O(1) palindrome checks -> precompute pal[i][j] = s[i] == s[j] and
#   (j - i < 2 or pal[i + 1][j - 1]), O(n^2)
# follow-up: min cuts only (LC 132) -> dp over the same pal table, O(n^2)
