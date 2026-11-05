// ByteDance (jobright, phone) LC 1573: split binary string s into 3 non-empty parts with
// the same number of '1's. Return the number of ways modulo 1e9 + 7.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Each part needs t = ones / 3 ones; cut 1 goes in the gap between the t-th and
// (t+1)-th one, cut 2 between the 2t-th and (2t+1)-th: multiply. T: O(n), S: O(n).
int numWays(const string& s) {
    long long mod = 1000000007;
    int n = s.size();
    // ones[i] = index of the (i+1)-th '1'
    vector<int> ones;
    for (int i = 0; i < n; i++) {
        if (s[i] == '1') {
            ones.push_back(i);
        }
    }
    int total = ones.size();
    if (total % 3 != 0) {
        return 0;
    }
    // no ones: pick any 2 of the n - 1 gaps
    if (total == 0) {
        return 1LL * (n - 1) * (n - 2) / 2 % mod;
    }
    int t = total / 3;
    long long firstCut = ones[t] - ones[t - 1];
    long long secondCut = ones[2 * t] - ones[2 * t - 1];
    return firstCut * secondCut % mod;
}

void test(const string& label, const string& s, int want) {
    cout << label << (numWays(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("gaps-multiply", "1001001", 9);
    test("adjacent-ones", "111", 1);
    test("not-divisible", "1010", 0);
    test("zeros-only", "00000", 6);
}

// follow-up: k parts -> product over j = 1..k-1 of (ones[j*t] - ones[j*t - 1]) mod p;
//   no ones: C(n - 1, k - 1) mod p
// follow-up: int array, 3 equal-sum parts -> S % 3 == 0; prefix sums, at j with
//   prefix[j] == 2S/3 add count of earlier i < j with prefix[i] == S/3, O(n)
// follow-up: O(1) space -> second pass records only ones[t-1], ones[t], ones[2t-1],
//   ones[2t]
