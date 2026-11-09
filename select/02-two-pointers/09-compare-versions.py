# LC 165 Compare Version Numbers (Medium). Pattern: two pointers, parse revisions.
# Versions are revisions joined by '.', e.g. "1.2.10"; each revision is a non-negative
# int (leading zeros allowed, "01" == "1"); a missing revision counts as 0.
# Return -1 if version1 < version2, 1 if version1 > version2, else 0.


# Walk both strings revision by revision, reading each number with a char loop; a
# finished string contributes 0. T: O(m + n), S: O(1).
def compare_version(version1, version2):
    # next unread char in version1 / version2
    pos1 = 0
    pos2 = 0
    len1 = len(version1)
    len2 = len(version2)
    while pos1 < len1 or pos2 < len2:
        rev1 = 0
        while pos1 < len1 and version1[pos1] != ".":
            rev1 = rev1 * 10 + int(version1[pos1])
            pos1 += 1
        rev2 = 0
        while pos2 < len2 and version2[pos2] != ".":
            rev2 = rev2 * 10 + int(version2[pos2])
            pos2 += 1
        if rev1 != rev2:
            return -1 if rev1 < rev2 else 1
        # skip the '.' after this revision
        pos1 += 1
        pos2 += 1
    return 0


def test(label, version1, version2, want):
    got = compare_version(version1, version2)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("numeric-not-text", "1.2.10", "1.2.9", 1)
    test("missing-is-zero", "3.0.0", "3", 0)
    test("leading-zeros", "2.05", "2.5", 0)
    test("shorter-smaller", "1.2", "1.2.1", -1)


main()

# follow-up: sort a list of versions -> versions.sort(key=functools.cmp_to_key(
#   compare_version)), O(k log k * L)
# follow-up: huge revisions -> Python ints don't overflow, so the loop still works; in
#   C++ strip leading zeros, compare by length, then as strings
