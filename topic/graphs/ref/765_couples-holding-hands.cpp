// LC 765 Couples Holding Hands: fewest swaps so each couple (2k, 2k + 1) sits in seats
// (2i, 2i + 1).
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Greedy with a seat map: for each seat pair, swap the left person's partner into the
// right seat. Each swap seats one couple for good, and couples tangled in a cycle of
// length c need c - 1 swaps, which this reaches. T: O(n), S: O(n).
int minSwapsCouples(vector<int> row) {
    int n = row.size();
    vector<int> seat(n);   // seat[p] = seat of person p
    for (int i = 0; i < n; i++) { seat[row[i]] = i; }

    int swaps = 0;
    for (int i = 0; i < n; i += 2) {
        // persons 2k and 2k + 1 differ only in the last bit
        int partner = row[i] ^ 1;
        if (row[i + 1] == partner) { continue; }

        int j = seat[partner];
        int other = row[i + 1];
        row[j] = other;
        seat[other] = j;
        row[i + 1] = partner;
        seat[partner] = i + 1;
        swaps++;
    }

    return swaps;
}

void test(const string& label, const vector<int>& row, int want) {
    cout << label << (minSwapsCouples(row) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("seated", {0, 1, 3, 2}, 0);
    test("three-cycle", {0, 2, 3, 4, 5, 1}, 2);
    test("two-cycles", {1, 2, 0, 3, 5, 6, 4, 7}, 2);
}
