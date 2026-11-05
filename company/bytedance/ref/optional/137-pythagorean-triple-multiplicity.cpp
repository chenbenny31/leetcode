// ByteDance: decide whether three elements at three different positions of nums have
// absolute values a, b, c with a^2 + b^2 = c^2. Signs do not matter; an absolute value
// can be used more than once only if the array holds it that many times (watch the
// zero cases, e.g. 0^2 + 7^2 = 7^2 needs two 7s). (n <= 2000, |nums[i]| <= 1e9.)

#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::map;

// Count each square, try every pair of distinct squares a <= b, look up a + b, then
// check multiplicities. Squares <= 1e18, sums <= 2e18 fit in long long.
// T: O(d^2 log d) for d distinct values, S: O(d).
bool hasPythagoreanTriple(const vector<int>& nums) {
    // count[sq] = how many elements have square sq
    map<long long, int> count;
    for (int x : nums) {
        long long v = x;
        count[v * v]++;
    }
    vector<long long> squares;
    for (auto& [sq, c] : count) {
        squares.push_back(sq);
    }

    for (int i = 0; i < (int)squares.size(); i++) {
        // j starts at i: a == b is allowed if it appears twice
        for (int j = i; j < (int)squares.size(); j++) {
            long long a = squares[i];
            long long b = squares[j];
            long long c = a + b;
            if (count.count(c) == 0) {
                continue;
            }

            // need[sq] = copies of sq this triple uses
            map<long long, int> need;
            need[a]++;
            need[b]++;
            need[c]++;
            bool fits = true;
            for (auto& [sq, k] : need) {
                if (count[sq] < k) {
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

// follow-up: O(n^2) without the map -> sort squares; for each k from the end, two
//   pointers i < j < k with sq[i] + sq[j] == sq[k]; distinct indices = multiplicity
// follow-up: |nums| up to 1e18 -> squares overflow long long; __int128 squares as keys
// follow-up: count all triples -> instead of returning, add the number of index
//   choices: C(count, need) per distinct square, multiplied
