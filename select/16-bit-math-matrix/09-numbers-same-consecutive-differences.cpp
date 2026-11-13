// LC 967 Numbers With Same Consecutive Differences (Medium) — digit-by-digit BFS / DFS
// return every n-digit num (no leading 0) whose adjacent digits all differ by exactly k
//
// clarify:
// 2 <= n <= 9 -> results fit in int
// 0 <= k <= 9; k = 0 -> must not emit dup values
// output order -> any
// only the cnt for huge n? -> follow-up (digit DP)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// core idea: BFS by len: start 1..9, extend w/ last + k and last - k if still a digit
// T: O(9 * 2^n), S: O(9 * 2^n) — each num has <= 2 children per round
vector<int> numsSameConsecDiff(int n, int k) {
    // level = every valid num w/ the current num of digits
    vector<int> level = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int length = 1; length < n; length++) {
        vector<int> nextLevel;
        for (int num : level) {
            int last = num % 10;
            if (last + k <= 9) { nextLevel.push_back(num * 10 + last + k); }
            // k == 0: last - k = digit just appended, skip the dup
            if (k > 0 && last - k >= 0) { nextLevel.push_back(num * 10 + last - k); }
        }
        level = nextLevel;
    }
    return level;
}

void test(const string& label, int n, int k, vector<int> want) {
    vector<int> got = numsSameConsecDiff(n, k);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("up-and-down", 2, 3, {14, 25, 30, 36, 41, 47, 52, 58, 63, 69, 74, 85, 96});
    test("k-zero-no-duplicates", 2, 0, {11, 22, 33, 44, 55, 66, 77, 88, 99});
    test("through-zero", 3, 9, {909});
    return 0;
}

// follow-up:
// stepping nums in [low, high] (LC 1215) -> same BFS, k = 1, from 0..9; stop above high
// sequential digits (LC 1291) -> start 1..9, append last + 1 while last < 9
// lexicographical nums (LC 386) -> DFS from 1..9 appending 0..9 while <= n (preorder)
// truncatable primes -> append a digit, keep the num only if prime
// "all nums <= n w/ property P" -> if P holds for every prefix, generate valid prefixes
// only the cnt, huge n -> digit DP over (pos, last digit, tight, started)
