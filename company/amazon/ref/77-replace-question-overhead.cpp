// Each '?' position has its own pair of replacement costs, and choices are independent.
// Return the min total overhead.

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::min;

// independent choices: take the cheaper option at every position. T: O(n), S: O(1).
long long minOverhead(const vector<pair<int, int>>& costs) {
    long long total = 0;
    for (const auto& [a, b] : costs) { total += min(a, b); }
    return total;
}

void test(const string& label, const vector<pair<int, int>>& costs, long long want) {
    cout << label << (minOverhead(costs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {{4, 1}, {2, 9}}, 3);
    test("tie", {{5, 5}}, 5);
    test("empty", {}, 0);
}
