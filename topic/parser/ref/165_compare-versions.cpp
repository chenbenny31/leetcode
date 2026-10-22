// LC 165 Compare Version Numbers: versions are dot-separated revisions. Compare them
// numerically left to right (leading zeros ignored, missing revisions count as 0).
// Return -1 if version1 < version2, 1 if version1 > version2, else 0.
#include <iostream>
#include <string>
using std::cout;
using std::string;

// Two pointers, parse one revision from each side per round. T: O(m+n), S: O(1).
int compareVersion(string version1, string version2) {
    int m = version1.size(), n = version2.size();
    int i = 0, j = 0;
    while (i < m || j < n) {
        // a finished side yields 0, so "1" equals "1.0.0"
        int a = 0, b = 0;
        while (i < m && version1[i] != '.') {
            a = a * 10 + (version1[i] - '0');
            i++;
        }
        while (j < n && version2[j] != '.') {
            b = b * 10 + (version2[j] - '0');
            j++;
        }
        if (a != b) { return a < b ? -1 : 1; }
        i++;
        j++;
    }
    return 0;
}

void test(const string& label, const string& v1, const string& v2, int want) {
    cout << label << (compareVersion(v1, v2) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("numeric-not-lexical", "1.2", "1.10", -1);
    test("leading-zeros", "2.007", "2.7", 0);
    test("missing-revisions", "3.0.0", "3", 0);
    test("longer-wins", "1.0.1", "1", 1);
}
