// LC 967 Numbers With Same Consecutive Differences (Medium) — build numbers digit by
//   digit (BFS / DFS)
// Return every n-digit number (no leading zero) whose adjacent digits all differ by
// exactly k, in any order.
// Clarify: 2 <= n <= 9 so results fit in int; 0 <= k <= 9; k = 0 must not repeat values.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Core idea: BFS by length: start from 1..9 and extend each number with last digit + k
// and last digit - k when that is still a digit; after n - 1 rounds the level is the
// answer. T: O(9 * 2^n), S: O(9 * 2^n) for the level.
vector<int> numsSameConsecDiff(int n, int k) {
    // level = every valid number that has the current number of digits
    vector<int> level = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int length = 1; length < n; length++) {
        vector<int> nextLevel;
        for (int num : level) {
            int last = num % 10;
            if (last + k <= 9) { nextLevel.push_back(num * 10 + last + k); }
            // k == 0: last - k is the digit just appended, skip the duplicate
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

// follow-up (ideas only):
// - stepping numbers in [low, high] (LC 1215) -> same BFS with k = 1 started from 0..9,
//   stop extending above high, keep the values that are >= low
// - sequential digits (LC 1291) -> start from 1..9, append last + 1 while last < 9
// - lexicographical numbers (LC 386) -> DFS from 1..9 appending 0..9 while <= n
//   (preorder is the lexicographic order)
// - truncatable primes -> append a digit, keep the number only if it is prime
// - "print all numbers <= n with property P" in general -> if P holds for every prefix,
//   generate the valid prefixes instead of testing every number
// - only the count, for huge n -> digit DP over (position, last digit, tight, started)
