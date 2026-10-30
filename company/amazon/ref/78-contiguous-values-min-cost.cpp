// One operation picks values x and y and changes every x into y; it costs the number of
// elements changed. Return the min total cost so that, in the final array, every value
// that remains occupies one contiguous block.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::max;

// Values whose [first, last] spans overlap must all end up as one value, so the array
// splits into independent segments (as in partition labels). In a segment, keep the most
// frequent value and change the rest. T: O(n), S: O(n).
long long minCost(const vector<int>& a) {
    int n = a.size();
    // last[x] = last index of x; cnt[x] = occurrences of x
    unordered_map<int, int> last, cnt;
    for (int i = 0; i < n; i++) {
        last[a[i]] = i;
        cnt[a[i]]++;
    }
    long long total = 0;
    int start = 0, end = 0, best = 0;
    for (int i = 0; i < n; i++) {
        end = max(end, last[a[i]]);
        best = max(best, cnt[a[i]]);
        if (i == end) {
            total += (end - start + 1) - best;
            start = i + 1;
            best = 0;
        }
    }
    return total;
}

void test(const string& label, const vector<int>& a, long long want) {
    cout << label << (minCost(a) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("already", {1, 1, 2}, 0);
    test("interleave", {1, 2, 1, 2}, 2);
    test("chain", {1, 2, 1, 3, 2}, 3);
    test("split", {1, 2, 1, 3, 3}, 1);
    test("single", {5}, 0);
}
