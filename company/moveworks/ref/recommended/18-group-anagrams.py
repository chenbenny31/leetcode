# 18 · Group Anagrams (ServiceNow, experienced hires, Medium) — LC 49
# group strs that are anagrams of each other; any order of groups and of words
#
# clarify:
# charset -> lowercase only; the sorted key works for any charset
# empty strs -> one group together
# dup words -> each copy stays
# output order -> any


# core idea: anagrams share one key = the word's letters sorted; dict key -> words
# T: O(n * k log k), S: O(n * k) — k = word len
def group_anagrams(strs):
    # groups[sorted letters] = words with those letters
    groups = {}
    for word in strs:
        key = "".join(sorted(word))
        if key not in groups:
            groups[key] = []
        groups[key].append(word)
    return list(groups.values())


# groups and the words inside them in sorted order, so any output order compares equal
def normalize(groups):
    result = [sorted(group) for group in groups]
    result.sort()
    return result


def test(label, strs, want):
    ok = normalize(group_anagrams(strs)) == normalize(want)
    print(label, "PASS" if ok else "FAIL")


def main():
    test("basic", ["ab", "c", "ba"], [["ab", "ba"], ["c"]])
    test("same-letters-diff-cnt", ["aab", "ab"], [["aab"], ["ab"]])
    test("empty-word", ["", "b", ""], [["", ""], ["b"]])


main()

# follow-up:
# 1. no sort, O(n * k) -> key = tuple of 26 letter cnts
# 2. group shifted strs (LC 249) -> key = (ord(w[i]) - ord(w[i - 1])) % 26 per pair
# 3. all anagram starts of p in s (LC 438) -> slide a window of len(p), compare 26 cnts
