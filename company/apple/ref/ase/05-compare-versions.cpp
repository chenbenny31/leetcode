// LC 165 Compare Version Numbers (ASE release engineering: artifact versions).
// Versions are revisions joined by '.', e.g. "1.2.10"; each revision is a non-negative
// integer (leading zeros allowed, "01" == "1"); a missing revision counts as 0.
// compareVersion returns -1 / 0 / 1. Follow-up: sort a list of versions ascending.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Two pointers walk both strings revision by revision, reading each number with a
// char loop; a finished string contributes 0. T: O(m + n), S: O(1).
int compareVersion(const string& a, const string& b) {
    int i = 0;
    int j = 0;
    int lenA = a.size();
    int lenB = b.size();
    while (i < lenA || j < lenB) {
        long long revA = 0;
        while (i < lenA && a[i] != '.') {
            revA = revA * 10 + (a[i] - '0');
            i++;
        }
        long long revB = 0;
        while (j < lenB && b[j] != '.') {
            revB = revB * 10 + (b[j] - '0');
            j++;
        }
        if (revA < revB) { return -1; }
        if (revA > revB) { return 1; }
        // skip the '.' after this revision
        i++;
        j++;
    }
    return 0;
}

// follow-up: sort versions ascending with the same comparison. T: O(k log k * L).
vector<string> sortVersions(vector<string> versions) {
    sort(versions.begin(), versions.end(), [](const string& a, const string& b) {
        return compareVersion(a, b) < 0;
    });
    return versions;
}

void test(const string& label, const string& a, const string& b, int want) {
    cout << label << (compareVersion(a, b) == want ? " PASS\n" : " FAIL\n");
}

void testSort(const string& label, const vector<string>& versions,
              const vector<string>& want) {
    cout << label << (sortVersions(versions) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("numeric-not-text", "1.2.10", "1.2.9", 1);
    test("missing-is-zero", "1.0.0", "1", 0);
    test("leading-zeros", "1.01", "1.001", 0);
    test("shorter-smaller", "1.2", "1.2.1", -1);
    testSort("sort", {"2.0", "1.10", "1.2", "1.2.0.1"},
             {"1.2", "1.2.0.1", "1.10", "2.0"});
}

// follow-up: semver "1.2.3-rc.1" -> compare major.minor.patch first; a pre-release sorts
//   before the plain release; compare pre-release parts left to right
// follow-up: latest version satisfying ">= 1.2, < 2.0" -> filter with compareVersion,
//   keep the max
// follow-up: huge revisions -> strip leading zeros, compare by length, then as strings
