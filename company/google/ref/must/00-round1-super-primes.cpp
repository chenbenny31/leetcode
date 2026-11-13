// 00 · Super Primes (Google round 1, 2026-11-12, his own) — primes + build by digits
// (1) isPrime(n)
// (2) isSuperPrime(n): n stays prime as last digit is removed; 239 -> 23 -> 2 yes, 20 no
// (3) return all super primes in [1, n]
//
// clarify:
// is 1 prime -> no, so 1 is not super either
// single-digit nums -> 2, 3, 5, 7 are super primes
// range of n -> sieve needs O(n) memory; huge n -> grow by digits
// output order -> ascending

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// (1) isPrime
// core idea: composite n has a divisor d w/ d * d <= n -> try 2..sqrt(n) only
// T: O(sqrt(n)), S: O(1)
bool isPrime(long long n) {
    if (n < 2) { return false; }
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) { return false; }
    }
    return true;
}

// (2) isSuperPrime
// core idea: test n, drop last digit, repeat; every prefix must be prime
// T: O(sqrt(n)), S: O(1) — prefixes shrink 10x each time
bool isSuperPrime(long long n) {
    if (n < 1) { return false; }
    while (n > 0) {
        if (!isPrime(n)) { return false; }
        n /= 10;
    }
    return true;
}

// (3) all super primes <= n
// solution 1: sieve + dp; simplest when n fits in memory
// core idea: sieve, then dp[i] = prime[i] && (i < 10 || dp[i / 10]); i / 10 known already
// T: O(n log log n), S: O(n) — sieve bound
vector<long long> superPrimesUpTo(int n) {
    if (n < 2) { return {}; }
    // prime[x] = x is prime
    vector<bool> prime(n + 1, true);
    prime[0] = false;
    prime[1] = false;
    for (int i = 2; i <= n; i++) {
        if (!prime[i]) { continue; }
        for (long long multiple = (long long)i * i; multiple <= n; multiple += i) {
            prime[multiple] = false;
        }
    }
    // dp[x] = x is a super prime
    vector<bool> dp(n + 1, false);
    vector<long long> result;
    for (int i = 2; i <= n; i++) {
        dp[i] = prime[i] && (i < 10 || dp[i / 10]);
        if (dp[i]) { result.push_back(i); }
    }
    return result;
}

// solution 2: grow by digits, no sieve; use it for huge n
// core idea: super prime = shorter super prime + 1 digit
// grow from 2, 3, 5, 7 by appending 1 / 3 / 7 / 9, keep primes <= n
// T: O(sqrt(n)), S: O(1) — only 83 exist (largest 73939133), <= 83 * 4 isPrime calls
vector<long long> superPrimesByGrowing(long long n) {
    int seeds[4] = {2, 3, 5, 7};
    int lastDigits[4] = {1, 3, 7, 9};
    // found = super primes <= n so far; found[i] is extended when loop reaches i (BFS)
    vector<long long> found;
    for (int seed : seeds) {
        if (seed <= n) { found.push_back(seed); }
    }
    for (size_t i = 0; i < found.size(); i++) {
        for (int digit : lastDigits) {
            long long nxt = found[i] * 10 + digit;
            if (nxt <= n && isPrime(nxt)) { found.push_back(nxt); }
        }
    }
    sort(found.begin(), found.end());
    return found;
}

void testPrime(const string& label, long long n, bool want) {
    cout << label << (isPrime(n) == want ? " PASS\n" : " FAIL\n");
}

void testSuper(const string& label, long long n, bool want) {
    cout << label << (isSuperPrime(n) == want ? " PASS\n" : " FAIL\n");
}

void testList(const string& label, int n, const vector<long long>& want) {
    bool ok = superPrimesUpTo(n) == want && superPrimesByGrowing(n) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    testPrime("one", 1, false);
    testPrime("two", 2, true);
    testPrime("odd-square", 9, false);
    testPrime("large-prime", 97, true);
    testSuper("three-digits", 239, true);
    testSuper("not-prime", 20, false);
    testSuper("ends-in-one", 19, false);
    testSuper("one-digit", 7, true);
    testList("up-to-100", 100, {2, 3, 5, 7, 23, 29, 31, 37, 53, 59, 71, 73, 79});
    testList("agree-5000", 5000, superPrimesByGrowing(5000));
    return 0;
}

// follow-up:
// his interview version -> same dp, but isPrime tried divisors up to n / 2: O(n^2) total
// why sqrt(n) is enough -> divisors pair as d * (n / d); smaller one <= sqrt(n)
// left-truncatable primes -> same dp w/ dp[i % 10^k] (no digit 0); or prepend 1..9
// two-sided primes -> filter the 83 right-truncatable ones w/ the left check
// cnt only -> precompute the 83 nums once, binary search n
// primality for 64-bit n -> Miller-Rabin w/ a fixed set of bases (deterministic)
