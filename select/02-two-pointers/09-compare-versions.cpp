// LC 165 Compare Version Numbers (Medium). Pattern: two pointers, parse revisions.
// Versions are revisions joined by '.', e.g. "1.2.10"; each revision is a non-negative
// int (leading zeros allowed, "01" == "1"); a missing revision counts as 0.
// Return -1 if version1 < version2, 1 if version1 > version2, else 0.

#include <iostream>
#include <string>

using std::cout;
using std::string;

// Walk both strings revision by revision, reading each number with a char loop; a
// finished string contributes 0. T: O(m + n), S: O(1).
int compareVersion(const string& version1, const string& version2) {
    // next unread char in version1 / version2
    int pos1 = 0;
    int pos2 = 0;
    int len1 = version1.size();
    int len2 = version2.size();
    while (pos1 < len1 || pos2 < len2) {
        int rev1 = 0;
        while (pos1 < len1 && version1[pos1] != '.') {
            rev1 = rev1 * 10 + (version1[pos1] - '0');
            pos1++;
        }
        int rev2 = 0;
        while (pos2 < len2 && version2[pos2] != '.') {
            rev2 = rev2 * 10 + (version2[pos2] - '0');
            pos2++;
        }
        if (rev1 != rev2) {
            return rev1 < rev2 ? -1 : 1;
        }
        // skip the '.' after this revision
        pos1++;
        pos2++;
    }
    return 0;
}

void test(const string& label, const string& version1, const string& version2,
          int want) {
    bool ok = compareVersion(version1, version2) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("numeric-not-text", "1.2.10", "1.2.9", 1);
    test("missing-is-zero", "3.0.0", "3", 0);
    test("leading-zeros", "2.05", "2.5", 0);
    test("shorter-smaller", "1.2", "1.2.1", -1);
}

// follow-up: sort a list of versions -> sort(v.begin(), v.end(), comparator lambda
//   returning compareVersion(x, y) < 0), O(k log k * L)
// follow-up: huge revisions -> strip leading zeros, compare by length, then as strings
