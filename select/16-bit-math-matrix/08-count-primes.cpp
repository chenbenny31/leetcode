// LC 204 Count Primes (Medium) — trial division + sieve of Eratosthenes
// isPrime(n): is n prime; countPrimes(n): num of primes strictly < n
//
// clarify:
// strictly below n or up to n -> strictly below (LC 204)
// 0 and 1 -> not prime
// how large is n -> sieve needs O(n) memory
// one query or many -> many: sieve once, O(1) each

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// (1) isPrime: trial division
// core idea: composite n has a divisor d w/ d * d <= n -> try 2..sqrt(n) only
// T: O(sqrt(n)), S: O(1)
bool isPrime(long long n) {
    if (n < 2) { return false; }
    // long long: i * i must not overflow
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) { return false; }
    }
    return true;
}

// (2) countPrimes: sieve of Eratosthenes
// core idea: unmarked i is prime; mark its multiples from i * i on
// T: O(n log log n), S: O(n) — harmonic sum over primes
int countPrimes(int n) {
    if (n < 3) { return 0; }
    // composite[x] = x has a prime factor smaller than itself
    vector<bool> composite(n, false);
    int count = 0;
    for (int i = 2; i < n; i++) {
        if (composite[i]) { continue; }
        count++;
        for (long long multiple = (long long)i * i; multiple < n; multiple += i) {
            composite[multiple] = true;
        }
    }
    return count;
}

void testPrime(const string& label, long long n, bool want) {
    cout << label << (isPrime(n) == want ? " PASS\n" : " FAIL\n");
}

void test(const string& label, int n, int want) {
    int byDivision = 0;
    for (int i = 0; i < n; i++) {
        if (isPrime(i)) { byDivision++; }
    }
    bool ok = countPrimes(n) == want && byDivision == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    testPrime("zero", 0, false);
    testPrime("one", 1, false);
    testPrime("two", 2, true);
    testPrime("odd-square", 9, false);
    testPrime("large-prime", 97, true);
    test("below-two", 2, 0);
    test("below-three", 3, 1);
    test("below-twelve", 12, 5);
    return 0;
}

// follow-up:
// right-truncatable ("super") primes <= n -> sieve, dp[i] = prime[i] && dp[i / 10]
// super primes for huge n -> grow from 2, 3, 5, 7 by appending 1 / 3 / 7 / 9, keep primes
// prime factorization -> i from 2 while i * i <= n: divide out i; leftover > 1 = factor
// gcd / lcm -> Euclid: gcd(a, b) = gcd(b, a % b), gcd(a, 0) = a; lcm = a / gcd * b
// one query w/ n up to 1e12 -> trial division up to 1e6 is enough
// many queries below a limit -> sieve once; prefix cnt for "primes in [lo, hi]"
