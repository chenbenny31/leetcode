// 09 · Number of Balanced Strings (Moveworks OA, Medium) — DP over the last letter
// count lowercase strings of len n where any two adjacent letters are at most d apart
// in the alphabet; answer mod 1e9+7. n = 2, d = 3 -> 170
//
// clarify:
// statement from a summary; its second example (n = 3, d = 3 -> 224) does not fit this
//   rule (the true count is far above 170), so confirm the statement
// mod -> assume 1e9+7
// n == 1 -> 26
// d >= 25 -> 26^n

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

const long long MOD = 1000000007LL;

// core idea: ways[c] = strings of the cur len ending in letter c; the next letter c2
// takes the sum of ways[c] for c within d of c2
// T: O(n * 26 * d), S: O(26)
long long countBalanced(int n, int d) {
    vector<long long> ways(26, 1);
    for (int len = 2; len <= n; len++) {
        vector<long long> next(26, 0);
        for (int c2 = 0; c2 < 26; c2++) {
            for (int c = max(0, c2 - d); c <= min(25, c2 + d); c++) {
                next[c2] = (next[c2] + ways[c]) % MOD;
            }
        }
        ways = next;
    }
    long long total = 0;
    for (long long w : ways) { total = (total + w) % MOD; }
    return total;
}

void test(const string& label, int n, int d, long long want) {
    cout << label << (countBalanced(n, d) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("example", 2, 3, 170);
    test("one-letter", 1, 3, 26);
    test("same-letter-only", 5, 0, 26);
    test("any-pair", 2, 25, 676);
    return 0;
}

// follow-up:
// n up to 1e18 -> the step is a fixed 26 x 26 matrix: fast matrix power, O(26^3 log n)
// faster step -> prefix sums over ways[], O(26) per len instead of O(26 * d)
// return one such string -> any constant string works ("aaa")
