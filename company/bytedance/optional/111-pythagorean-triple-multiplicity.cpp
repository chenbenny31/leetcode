// ByteDance: decide whether three elements at three different positions of nums have
// absolute values a, b, c with a^2 + b^2 = c^2. Signs do not matter; an absolute value
// can be used more than once only if the array holds it that many times (watch the
// zero cases, e.g. 0^2 + 7^2 = 7^2 needs two 7s). (n <= 2000, |nums[i]| <= 1e9.)

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::sqrt;

// Exact integer square root (floor) of x >= 0.
long long isqrt(long long x) {
    long long r = (long long)sqrt((long double)x);
    while (r * r > x) {
        r--;
    }
    while ((r + 1) * (r + 1) <= x) {
        r++;
    }
    return r;
}

// Count each absolute value, then try every pair a <= b of distinct values; c must be
// an exact square root that is present, and the triple must fit the multiplicities.
// Squares reach 1e18 and their sum 2e18, still inside long long.
// T: O(d^2 log d) for d distinct values, S: O(d).
bool hasPythagoreanTriple(const vector<int>& nums) {
    // count[v] = how many elements have absolute value v
    map<long long, int> count;
    for (int x : nums) {
        long long v = x;
        if (v < 0) {
            v = -v;
        }
        count[v]++;
    }
    vector<long long> values;
    for (auto& [v, c] : count) {
        values.push_back(v);
    }

    for (int i = 0; i < (int)values.size(); i++) {
        for (int j = i; j < (int)values.size(); j++) {
            long long a = values[i];
            long long b = values[j];
            long long c = isqrt(a * a + b * b);
            if (c * c != a * a + b * b || count.count(c) == 0) {
                continue;
            }

            // need[v] = copies of v this triple uses
            map<long long, int> need;
            need[a]++;
            need[b]++;
            need[c]++;
            bool fits = true;
            for (auto& [v, k] : need) {
                if (count[v] < k) {
                    fits = false;
                }
            }
            if (fits) {
                return true;
            }
        }
    }
    return false;
}

void test(const string& label, const vector<int>& nums, bool want) {
    cout << label << (hasPythagoreanTriple(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("signs", {-8, 15, 17}, true);
    test("zero-needs-pair", {0, 7, 7}, true);
    test("zero-single", {0, 7}, false);
    test("triple-zero", {0, 0, 0}, true);
    test("none", {2, 3, 4}, false);
}
