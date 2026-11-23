// 15 · Count Numbers With Digit Sum Y (Cisco OA, Medium) — brute force / digit DP
// count integers in [1, X] whose digits sum to Y; -1 if there are none
// X = 20, Y = 5 -> {5, 14} -> 2
//
// clarify:
// range starts at 1 -> 0 is not counted
// X range -> unknown; brute force is fine to ~1e7, digit DP for anything up to 1e18
// Y range -> Y > 9 * (num of digits of X) means none -> -1
// none -> -1, not 0 (given)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;
using std::to_string;

// solution 1: check every num; say it first, then offer the DP
// T: O(X log X), S: O(1)
long long countBrute(long long x, int y) {
    long long count = 0;
    for (long long v = 1; v <= x; v++) {
        int sum = 0;
        for (long long t = v; t > 0; t /= 10) { sum += t % 10; }
        if (sum == y) { count++; }
    }
    return count == 0 ? -1 : count;
}

// nums formed by digits[pos..] with digit sum `left`; tight = prefix equals X's so far
long long countFrom(const string& digits, int pos, int left, bool tight,
                    vector<vector<long long>>& memo) {
    if (left < 0) { return 0; }
    if (pos == (int)digits.size()) { return left == 0 ? 1 : 0; }
    // only the non-tight states repeat, so only they are cached
    if (!tight && memo[pos][left] != -1) { return memo[pos][left]; }
    int limit = tight ? digits[pos] - '0' : 9;
    long long total = 0;
    for (int d = 0; d <= limit; d++) {
        total += countFrom(digits, pos + 1, left - d, tight && d == limit, memo);
    }
    if (!tight) { memo[pos][left] = total; }
    return total;
}

// solution 2: digit DP over X's digits, most significant first; counts 0..X, so drop
// the num 0 when Y == 0
// T: O(D * Y * 10), S: O(D * Y) — D = num of digits of X
long long countDigitSum(long long x, int y) {
    string digits = to_string(x);
    int d = digits.size();
    if (y > 9 * d) { return -1; }
    // memo[pos][left] = count for a non-tight prefix, -1 = not computed
    vector<vector<long long>> memo(d, vector<long long>(y + 1, -1));
    long long count = countFrom(digits, 0, y, true, memo);
    if (y == 0) { count--; }
    return count == 0 ? -1 : count;
}

void test(const string& label, long long x, int y, long long want) {
    bool ok = countDigitSum(x, y) == want && countBrute(x, y) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("example", 20, 5, 2);
    test("powers-of-ten", 1000, 1, 4);
    test("none", 9, 10, -1);
    test("bigger", 12345, 15, countBrute(12345, 15));
    return 0;
}

// follow-up:
// 1. range [L, R] -> count(R) - count(L - 1)
// 2. digit sum divisible by m -> state (pos, sum % m) instead of the exact sum
// 3. smallest num with digit sum Y -> greedy: fill 9s from the right
// 4. why memo skips tight states -> a tight prefix happens once per position, no reuse
