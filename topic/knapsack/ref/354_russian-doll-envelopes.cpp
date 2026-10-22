// LC 354 Russian Doll Envelopes: envelopes[i] = {width, height}. One fits inside another
// only if both width and height are strictly smaller. Return the longest nesting chain.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::lower_bound;

// Sort by width ascending, ties by height descending (equal widths cannot chain), then
// strict LIS on heights with patience piles. T: O(n log n), S: O(n).
int maxEnvelopes(vector<vector<int>> envelopes) {
    sort(envelopes.begin(), envelopes.end(),
         [](const vector<int>& a, const vector<int>& b) {
             return a[0] != b[0] ? a[0] < b[0] : a[1] > b[1];
         });
    // tails[k] = smallest height ending a chain of length k + 1
    vector<int> tails;
    for (const auto& e : envelopes) {
        auto it = lower_bound(tails.begin(), tails.end(), e[1]);
        if (it == tails.end()) {
            tails.push_back(e[1]);
        } else {
            *it = e[1];
        }
    }
    return tails.size();
}

void test(const string& label, const vector<vector<int>>& envelopes, int want) {
    cout << label << (maxEnvelopes(envelopes) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {{4, 5}, {4, 6}, {6, 7}, {2, 3}}, 3);
    test("same-width", {{1, 3}, {1, 4}, {1, 5}}, 1);
    test("equal", {{2, 2}, {2, 2}}, 1);
    test("empty", {}, 0);
}
