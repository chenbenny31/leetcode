// LC 204 Count Primes: return the number of primes strictly less than n.
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::string;
using std::vector;

// Sieve of Eratosthenes: each prime i crosses out its multiples, starting at i * i
// (smaller multiples have a smaller prime factor). T: O(n log log n), S: O(n).
int countPrimes(int n) {
    vector<bool> composite(n, false);   // composite[i] = i has a smaller prime factor
    int cnt = 0;
    for (int i = 2; i < n; i++) {
        if (composite[i]) { continue; }
        cnt++;
        for (long long j = 1LL * i * i; j < n; j += i) { composite[j] = true; }
    }
    return cnt;
}

void test(const string& label, int n, int want) {
    cout << label << (countPrimes(n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("twenty", 20, 8);
    test("strictly-less", 3, 1);
    test("two", 2, 0);
    test("zero", 0, 0);
}
