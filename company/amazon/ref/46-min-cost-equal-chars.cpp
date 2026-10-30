// LC 2712: s is a binary string of length n. Inverting s[0..i] costs i + 1; inverting
// s[i..n-1] costs n - i. Return the min cost to make all characters equal.

#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::min;

// Each boundary s[i-1] != s[i] must be removed by exactly one flip that ends there:
// the prefix flip of length i or the suffix flip of length n - i, and boundaries don't
// interact, so pay the cheaper one for each. T: O(n), S: O(1).
long long minimumCost(const string& s) {
    long long n = s.size(), cost = 0;
    for (int i = 1; i < n; i++) {
        if (s[i] != s[i - 1]) { cost += min<long long>(i, n - i); }
    }
    return cost;
}

void test(const string& label, const string& s, long long want) {
    cout << label << (minimumCost(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", "0011", 2);
    test("two-edges", "0110", 2);
    test("pair", "01", 1);
    test("same", "1111", 0);
    test("single", "0", 0);
}
