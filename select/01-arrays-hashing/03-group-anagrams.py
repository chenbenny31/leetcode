# LC 49 Group Anagrams (Medium). Pattern: hash map with canonical key.
# Group the lowercase strings that are anagrams of each other; any order of groups and
# of words within a group.

from collections import defaultdict


# Key = the word's letters sorted. T: O(n * k log k), S: O(n * k); k = max word length.
def group_anagrams(strs):
    # sorted letters -> words with those letters
    groups = defaultdict(list)
    for word in strs:
        key = "".join(sorted(word))
        groups[key].append(word)
    return list(groups.values())


# Follow-up: key = 26 letter counts, no sort. T: O(n * k), S: O(n * k).
def group_anagrams_count(strs):
    # count key -> words with those letter counts
    groups = defaultdict(list)
    for word in strs:
        # letter_count[i] = occurrences of letter 'a' + i
        letter_count = [0] * 26
        for c in word:
            letter_count[ord(c) - ord("a")] += 1
        # a tuple is hashable and keeps counts 1,11 distinct from 11,1 (no separator)
        groups[tuple(letter_count)].append(word)
    return list(groups.values())


# canonical form for comparing: sort words inside each group, then sort the groups
def normalize(groups):
    sorted_groups = [sorted(group) for group in groups]
    sorted_groups.sort()
    return sorted_groups


def test(label, strs, want):
    ok = normalize(group_anagrams(strs)) == normalize(want)
    ok = ok and normalize(group_anagrams_count(strs)) == normalize(want)
    print(label, "PASS" if ok else "FAIL")


def main():
    test("basic", ["ab", "c", "ba"], [["ab", "ba"], ["c"]])
    test("same-letters-diff-count", ["aab", "ab"], [["aab"], ["ab"]])
    test("empty-word", ["", "b", ""], [["", ""], ["b"]])


main()

# follow-up: all anagram starts of p in s (LC 438) -> fixed window |p| with letter
#   counts, compare counts each step, O(26 n)
# follow-up: any charset -> sorted key still works; count key becomes
#   tuple(sorted(Counter(word).items()))
