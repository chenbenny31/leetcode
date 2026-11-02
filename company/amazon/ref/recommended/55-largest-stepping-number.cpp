// Largest stepping number: every pair of adjacent digits differs by exactly 1 (all
// one-digit numbers, including 0, count). Return the largest one <= limit.

#include <iostream>
#include <string>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::queue;
using std::max;

// BFS builds stepping numbers by appending last digit +/- 1; stop growing past limit.
// T: O(S) for S stepping numbers <= limit (about 10 * 2^digits), S: O(S).
long long largestSteppingNumber(long long limit) {
    long long best = 0;
    queue<long long> q;
    for (int d = 1; d <= 9; d++) { q.push(d); }
    while (!q.empty()) {
        long long cur = q.front();
        q.pop();
        if (cur > limit) { continue; }
        best = max(best, cur);
        int last = cur % 10;
        if (last > 0) { q.push(cur * 10 + last - 1); }
        if (last < 9) { q.push(cur * 10 + last + 1); }
    }
    return best;
}

void test(const string& label, long long limit, long long want) {
    cout << label << (largestSteppingNumber(limit) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("zero", 0, 0);
    test("one-digit", 7, 7);
    test("itself", 4321, 4321);
    test("drop-down", 200, 123);
}
