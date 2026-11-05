// ByteDance (hack2hire): sqrt of a non-negative int val to `precise` decimal places,
// truncated, as a string with exactly `precise` decimals, e.g. (2, 3) -> "1.414".

#include <iostream>
#include <string>

using std::cout;
using std::string;
using std::to_string;

// Binary search the integer part, then long-hand digit by digit with exact integers.
// T: O(log val + p), S: O(p).
string sqrtPrecise(int val, int precise) {
    // floor(sqrt(val)) lies in [lo, hi]
    long long lo = 0;
    long long hi = val;
    while (lo < hi) {
        // round up so lo = mid always makes progress
        long long mid = (lo + hi + 1) / 2;
        if (mid * mid <= val) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    // r = digits so far = floor(sqrt(val * 100^d)); rem = val * 100^d - r^2
    long long r = lo;
    long long rem = val - r * r;
    string result = to_string(r);
    if (precise > 0) {
        result += '.';
    }
    for (int d = 0; d < precise; d++) {
        rem *= 100;
        // largest digit x with (10r + x)^2 <= 100 * r^2 + rem, i.e. (20r + x) * x <= rem
        int x = 9;
        while ((20 * r + x) * x > rem) {
            x--;
        }
        rem -= (20 * r + x) * x;
        r = r * 10 + x;
        result += char('0' + x);
    }
    return result;
}

void test(const string& label, int val, int precise, const string& want) {
    cout << label << (sqrtPrecise(val, precise) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("irrational", 2, 3, "1.414");
    test("perfect-square", 4, 2, "2.00");
    test("zero", 0, 5, "0.00000");
    test("no-decimals", 8, 0, "2");
}

// follow-up: Newton instead -> x = (x + val / x) / 2 in long double, quadratic
//   convergence; floating error can flip the last truncated digit
// follow-up: large precise (r * 20 * 100 overflows long long) -> __int128 or
//   digit-string big ints for r and rem, O(p^2)
// follow-up: nth root -> binary search largest y with y^n <= val * 10^(n * p); print y
//   with p decimals (big ints / __int128 for the scale)
