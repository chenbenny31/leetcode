// Return the min number of bills and coins that make an amount exactly, with bills 20,
// 10, 5, 1 and coins 0.25, 0.10, 0.05, 0.01. Work in integer cents.
// Follow-up: each denomination has a limited count in the cash drawer; return the min
// count, or -1 if the amount can't be made.

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// denominations in cents, largest first
const vector<int> DENOMS = {2000, 1000, 500, 100, 25, 10, 5, 1};

// Greedy is optimal for this US set: take as many of the largest as fit, and stop once
// nothing is left. T: O(denominations), S: O(1).
long long minPieces(long long cents) {
    long long pieces = 0;
    for (int d : DENOMS) {
        if (cents == 0) { break; }
        pieces += cents / d;
        cents %= d;
    }
    return pieces;
}

// follow-up: with limited counts greedy can fail, so run a bounded knapsack over
// (denomination, amount). drawer[i] = pieces of DENOMS[i] available.
// T: O(cents * total pieces), S: O(cents).
int minPiecesLimited(int cents, const vector<int>& drawer) {
    // best[a] = min pieces making exactly a cents
    vector<int> best(cents + 1, INT_MAX);
    best[0] = 0;
    int k = DENOMS.size();
    for (int i = 0; i < k; i++) {
        vector<int> next = best;
        for (int a = 0; a <= cents; a++) {
            if (best[a] == INT_MAX) { continue; }
            for (int c = 1; c <= drawer[i] && a + c * DENOMS[i] <= cents; c++) {
                next[a + c * DENOMS[i]] = min(next[a + c * DENOMS[i]], best[a] + c);
            }
        }
        best = next;
    }
    if (best[cents] == INT_MAX) { return -1; }
    return best[cents];
}

void test(const string& label, long long cents, long long want) {
    cout << label << (minPieces(cents) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", 4199, 12);
    test("coins", 30, 2);
    test("one-bill", 2000, 1);
    test("zero", 0, 0);
}
