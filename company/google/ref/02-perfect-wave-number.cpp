// A perfect wave number has no digit 0, no repeated digit, and zigzags: every inner
// digit is either higher than both neighbors or lower than both (123 is not one,
// 132 and 2413 are). Write isPerfectWave(n).
// Follow-up: count perfect wave numbers in [1, N] (e.g. N = 9999).
// ("wake" in the report read as "wave"; the stated rule taken as peak-or-valley.)

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
        if (used >> d & 1) { continue; }
        if (dir == 1 && d > last) { continue; }
        if (dir == -1 && d < last) { continue; }
        int nextDir = d > last ? 1 : -1;
        dfs(value * 10 + d, d, nextDir, used | 1 << d, N, count);
    }
}

// follow-up, improved: backtracking visits only perfect wave numbers <= N (plus one
// rejected child each). T: O(W * 9) for W such numbers, S: O(9) recursion depth.
int countWavesBacktrack(int N) {
    int count = 0;
    for (int d = 1; d <= 9; d++) { dfs(d, d, 0, 1 << d, N, count); }
    return count;
}

void test(const string& label, int n, bool want) {
    cout << label << (isPerfectWave(n) == want ? " PASS\n" : " FAIL\n");
}

void testCount(const string& label, int N, int want) {
    bool ok = countWaves(N) == want && countWavesBacktrack(N) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
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
}
