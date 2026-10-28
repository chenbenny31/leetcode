// Cities 0..n-1 have populations; unit[i] == '1' means city i starts with a security
// unit. A unit may stay or move one city left (the unit in city 0 can only stay). A city
// is protected if at least one unit ends there. Return the max total population of
// protected cities.

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Scan left to right. covered = whether the previous city is protected so far; only the
// unit of the current city can still change that by moving left.
// T: O(n), S: O(1).
long long maxProtected(const vector<int>& population, const string& unit) {
    const long long NONE = LLONG_MIN / 2;
    // best[c] = best total for cities so far, where c = whether the last one is protected
    long long best0 = 0, best1 = NONE;
    int n = population.size();
    for (int i = 0; i < n; i++) {
        long long any = max(best0, best1);
        long long next0 = any, next1 = NONE;
        if (unit[i] == '1') {
            // stay: city i protected
            next1 = any + population[i];
            // move left: city i - 1 becomes protected (gain only if it wasn't)
            if (i > 0) { next0 = max(best0 + population[i - 1], best1); }
        }
        best0 = next0;
        best1 = next1;
    }
    return max(best0, best1);
}

void test(const string& label, const vector<int>& population, const string& unit,
          long long want) {
    cout << label << (maxProtected(population, unit) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("stay", {5, 10, 3}, "010", 10);
    test("move-left", {1, 100, 1, 1}, "0010", 100);
    test("both", {4, 1, 4}, "011", 8);
    test("no-units", {3, 3, 3}, "000", 0);
    test("first-city", {7}, "1", 7);
}
