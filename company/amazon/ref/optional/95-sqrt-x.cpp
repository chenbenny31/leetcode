// LC 69: return floor(sqrt(x)) for x >= 0 without a sqrt function.

#include <iostream>
#include <string>

using std::cout;
using std::string;

// binary search the largest r with r * r <= x (64-bit to avoid overflow).
// T: O(log x), S: O(1).
int mySqrt(int x) {
    long long lo = 0, hi = x;
    while (lo <= hi) {
        long long mid = lo + (hi - lo) / 2;
        if (mid * mid <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

void test(const string& label, int x, int want) {
    cout << label << (mySqrt(x) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("zero", 0, 0);
    test("one", 1, 1);
    test("floor", 8, 2);
    test("square", 16, 4);
    test("big", 2147395599, 46339);
}
