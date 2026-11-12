// A perfect wave number has no digit 0, no repeated digit, and zigzags: every inner
// digit is either higher than both neighbors or lower than both (123 is not one,
// 132 and 2413 are). Write isPerfectWave(n).
// Follow-up: count perfect wave numbers in [1, N] (e.g. N = 9999).
// ("wake" in the report read as "wave"; the stated rule taken as peak-or-valley.)
//
// Clarify:
// - n >= 1? -> yes; 0 and negatives are out of scope
// - one digit -> a wave (no inner digit); two digits -> any two distinct nonzero digits
// - zigzag is strict: a digit equal to a neighbor fails (no-repeat already rules it out)
// - [1, N] includes N
// - N range -> distinct nonzero digits cap a wave number at 9 digits and only 76,343
//   exist in total, so backtracking is fast for any N; brute force is O(N * d)
// Idea: isPerfectWave: take the digits, check nonzero + distinct with a seen array, then
//   every inner digit is a peak or a valley. Count: build numbers digit by digit keeping
//   only valid prefixes (each step turns against the previous one); every valid prefix
//   is a wave number, and appending a digit only makes it bigger, so stop once past N.
// Pitfalls:
// - one- and two-digit numbers have no inner digit: loop i from 1 while i + 1 < len
// - after the first digit dir = 0: the second digit may go up or down
// - test value > N before counting it; keep value long long (with distinct digits it
//   stays below 1e9, but the repeat / digit-0 follow-ups pass INT_MAX)
// - parenthesize bit tests: (used >> d) & 1; `x & 1 == 0` parses as x & (1 == 0)
// - the counting DFS visits in prefix order (1, 12, 13, ..., 2), not numeric order
// Tests to write: rising run (123) -> false; a peak; a 4-digit zigzag; contains 0; a
//   repeated digit; one digit; counts for N = 9, 99, a bound inside the 3-digit waves,
//   9999; the sorted list across the 1- to 2-digit boundary.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Read the digits, then check the three rules in one pass. T: O(d) for d digits, S: O(d).
bool isPerfectWave(int n) {
    // digits of n, most significant first
    vector<int> d;
    for (int x = n; x > 0; x /= 10) { d.insert(d.begin(), x % 10); }
    bool seen[10] = {};
    int len = d.size();
    for (int i = 0; i < len; i++) {
        if (d[i] == 0 || seen[d[i]]) { return false; }
        seen[d[i]] = true;
    }
    for (int i = 1; i + 1 < len; i++) {
        bool peak = d[i] > d[i - 1] && d[i] > d[i + 1];
        bool valley = d[i] < d[i - 1] && d[i] < d[i + 1];
        if (!peak && !valley) { return false; }
    }
    return true;
}

// follow-up, straightforward: test every number. T: O(N * d), S: O(d).
int countWaves(int N) {
    int count = 0;
    for (int x = 1; x <= N; x++) {
        if (isPerfectWave(x)) { count++; }
    }
    return count;
}

// Build numbers digit by digit, only ever making valid prefixes: the first two digits
// fix a direction and every later step must turn the other way. Every valid prefix is
// itself a perfect wave number, and extending only makes it bigger, so stop past N.
// dir: 0 = one digit so far, 1 = last step went up, -1 = last step went down.
void dfs(long long value, int last, int dir, int used, int N, int& count) {
    if (value > N) { return; }
    count++;
    for (int d = 1; d <= 9; d++) {
        if ((used >> d) & 1) { continue; }
        if (dir == 1 && d > last) { continue; }
        if (dir == -1 && d < last) { continue; }
        int nextDir = d > last ? 1 : -1;
        dfs(value * 10 + d, d, nextDir, used | (1 << d), N, count);
    }
}

// follow-up, improved: backtracking visits only perfect wave numbers <= N (plus one
// rejected child each). T: O(W * 9) for W such numbers, S: O(9) recursion depth.
int countWavesBacktrack(int N) {
    int count = 0;
    for (int d = 1; d <= 9; d++) { dfs(d, d, 0, 1 << d, N, count); }
    return count;
}

// append to out, in increasing order, every wave number <= N that extends the prefix
// value by exactly `left` more digits
void dfsOrdered(long long value, int last, int dir, int used, int left, int N,
                vector<int>& out) {
    if (left == 0) {
        if (value <= N) { out.push_back((int)value); }
        return;
    }
    for (int d = 1; d <= 9; d++) {
        if ((used >> d) & 1) { continue; }
        if (dir == 1 && d > last) { continue; }
        if (dir == -1 && d < last) { continue; }
        int nextDir = d > last ? 1 : -1;
        dfsOrdered(value * 10 + d, d, nextDir, used | (1 << d), left - 1, N, out);
    }
}

// follow-up "list them in increasing order": the counting DFS emits prefix order (1, 12,
// 13, ..., 2). A shorter number is smaller, and at one length trying digits in
// increasing order gives increasing numbers, so run one fixed-length DFS per length,
// stopping at the first length whose smallest number exceeds N.
// T: O(9 * W) for W wave numbers of up to N's length, S: O(9) recursion + output.
vector<int> wavesInOrder(int N) {
    vector<int> out;
    // smallest number with len digits
    long long low = 1;
    for (int len = 1; len <= 9 && low <= N; len++) {
        for (int d = 1; d <= 9; d++) { dfsOrdered(d, d, 0, 1 << d, len - 1, N, out); }
        low *= 10;
    }
    return out;
}

void test(const string& label, int n, bool want) {
    cout << label << (isPerfectWave(n) == want ? " PASS\n" : " FAIL\n");
}

void testCount(const string& label, int N, int want) {
    bool ok = countWaves(N) == want && countWavesBacktrack(N) == want;
    ok = ok && (int)wavesInOrder(N).size() == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testOrder(const string& label, int N, const vector<int>& want) {
    cout << label << (wavesInOrder(N) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("rising", 123, false);
    test("peak", 132, true);
    test("zigzag", 2413, true);
    test("zero", 1203, false);
    test("repeat", 121, false);
    test("one-digit", 7, true);
    testCount("one-digit-all", 9, 9);
    testCount("two-digits", 99, 81);
    testCount("bound-inside", 132, 82);
    testCount("four-digits", 9999, 1677);
    testOrder("order-one-digit", 5, {1, 2, 3, 4, 5});
    testOrder("order-cross-length", 21,
              {1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15, 16, 17, 18, 19, 21});
}

// follow-up: count in [lo, hi] -> countWavesBacktrack(hi) - countWavesBacktrack(lo - 1)
// follow-up: N up to 1e18 -> a wave number has at most 9 digits and there are only
//   76,343 of them, so the backtracking stays tiny; just take N as long long
// follow-up: list them in increasing order -> wavesInOrder above
// follow-up: repeated digits allowed (still no 0, strict zigzag) -> drop `used`; the
//   mask was also what stopped equal neighbors (dir 0 would let 11 through), so add:
//     if (d == last) { continue; }
//   numbers now get as long as N, so for N up to 1e18 use a digit DP over
//   (position, last digit, direction, tight) instead of listing them
// follow-up: digit 0 allowed except in front -> dfs loops d from 0; the first digit still
//   comes from 1..9 in countWavesBacktrack; up to 10 digits, so value needs long long
