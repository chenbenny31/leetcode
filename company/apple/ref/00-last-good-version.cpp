// Versions 1..n, once a version is bad every later one
// is bad, and n is known to be bad. Using isBad(v), return the last good version, or 0
// if every version is bad.
#include <iostream>
#include <string>
using std::cout;
using std::string;

int firstBad = 1;   // test stub: versions firstBad..n are bad

// given API
bool isBad(int v) { return v >= firstBad; }

// Binary search for the first bad version; the last good one is just before it.
// T: O(log n) isBad calls, S: O(1).
int lastGood(int n) {
    int lo = 1, hi = n;   // first bad version lies in [lo, hi]
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (isBad(mid)) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    // lo = first bad; all bad -> lo == 1 -> 0, no special case needed
    return lo - 1;
}

void test(const string& label, int n, int bad, int want) {
    firstBad = bad;
    cout << label << (lastGood(n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mid", 5, 3, 2);
    test("only-n", 3, 3, 2);
    test("all-bad", 3, 1, 0);
    test("one", 1, 1, 0);
}
