// n (even) servers have memory[i]. Split them into n/2 (primary, backup) pairs with
// memory[backup] >= memory[primary]. Return the max total memory of the primaries.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Each pair's primary is its smaller server, so the primaries can't beat the n/2
// smallest values overall, and pairing sorted[i] with sorted[i + n/2] achieves that.
// T: O(n log n), S: O(1) extra.
long long maxPrimaryMemory(vector<int> memory) {
    sort(memory.begin(), memory.end());
    int n = memory.size();
    long long total = 0;
    for (int i = 0; i < n / 2; i++) { total += memory[i]; }
    return total;
}

void test(const string& label, const vector<int>& memory, long long want) {
    cout << label << (maxPrimaryMemory(memory) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("unsorted", {8, 2, 6, 4}, 6);
    test("equal", {3, 3}, 3);
    test("dups", {1, 5, 5, 5}, 6);
}
