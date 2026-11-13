// Perfect Wave Number (Google, interview report)
// (1) isPerfectWave(n): no digit 0, no repeated digit, every inner digit a peak or valley
// (1) 132 and 2413 are, 123 is not
// (2) count the perfect wave numbers in [1, N] (e.g. N = 9999)
//
// clarify:
// zigzag -> strict peak or valley (report's "wake" read as "wave")
// n -> >= 1
// 1- and 2-digit numbers -> no inner digit; only nonzero + distinct apply
// [1, N] -> includes N
// range of N -> int here; long long is a follow-up

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// (1) check one number
// core idea: take digits; nonzero + distinct via seen array; inner digit = peak or valley
// T: O(d), S: O(d) — d = num of digits
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

// (2) solution 1: test every number; baseline
// core idea: isPerfectWave(x) for every x in 1..N
// T: O(N * d), S: O(d)
int countWaves(int N) {
    int count = 0;
    for (int x = 1; x <= N; x++) {
        if (isPerfectWave(x)) { count++; }
    }
    return count;
}

// dir: 0 = one digit so far, 1 = last step went up, -1 = last step went down
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

// (2) solution 2: backtracking over valid prefixes; for large N
// core idea: build digit by digit, each step turns against the previous one
// every valid prefix is a wave number and extending only grows it, so stop past N
// T: O(9 * W), S: O(9) — W = wave numbers <= N
int countWavesBacktrack(int N) {
    int count = 0;
    for (int d = 1; d <= 9; d++) { dfs(d, d, 0, 1 << d, N, count); }
    return count;
}

// append, in increasing order, every wave number <= N extending value by left digits
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

// (2) solution 3: one fixed-len DFS per len; lists them in increasing order
// core idea: the counting DFS emits prefix order (1, 12, 13, ..., 2)
// shorter = smaller, and at one len increasing digits give increasing numbers
// T: O(9 * W), S: O(9) + output — W = wave numbers of up to N's len
vector<int> wavesInOrder(int N) {
    vector<int> out;
    // smallest number w/ len digits
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
    test("zigzag", 2413, true);
    test("zero", 1203, false);
    test("repeat", 121, false);
    testCount("one-digit-all", 9, 9);
    testCount("two-digits", 99, 81);
    testCount("bound-inside", 132, 82);
    testCount("four-digits", 9999, 1677);
    testOrder("order-one-digit", 5, {1, 2, 3, 4, 5});
    testOrder("order-cross-length", 21,
              {1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15, 16, 17, 18, 19, 21});
}

// follow-up:
// count in [lo, hi] -> countWavesBacktrack(hi) - countWavesBacktrack(lo - 1)
// N up to 1e18 -> <= 9 digits, only 76,343 wave numbers exist; just take N as long long
// repeated digits allowed (no 0, strict zigzag) -> drop used mask but skip d == last
// repeats + huge N -> digit DP over (position, last digit, direction, tight)
// digit 0 allowed except in front -> loop d from 0 after the 1st digit; value long long
