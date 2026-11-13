// Double and Shuffle / Recover Original (Google, interview report), two parts.
// (1) doubleAndShuffle(nums): nums followed by 2 * each element, uniformly shuffled.
// (2) recoverOriginal(changed): return any array a such that a + its doubles, shuffled,
//     is changed (listed in ascending |value| order); empty if impossible. Values may be
//     negative; zeros pair with zeros; doubled values are assumed to fit in int.
//
// Clarify (1): uniform = every order equally likely -> Fisher-Yates; empty nums -> {}.
// Pitfalls (1): swap slot i with a random slot in [0, i] (rand() % (i + 1)); picking from
//   the whole array every time is biased. rand() % m is also slightly biased: real code
//   uses mt19937 with uniform_int_distribution.
// Clarify (2):
// - odd length -> impossible, {}
// - negatives -> yes; the double of -3 is -6, the one with the larger |value|
// - zeros -> pair with each other, so their count must be even
// - duplicates -> yes, kept in a count map
// - output order -> any (here ascending |value|); the answer is unique as a multiset,
//   since every step below is forced
// - n up to 1e5 -> O(n log n) for the sort
// Idea (2): the remaining value with the smallest |x| cannot be anyone's double (its half
//   would be smaller), so it is an original and uses up one copy of 2x. Walk the values
//   in |x| order with a count map, skipping copies already used as doubles.
// Pitfalls (2):
// - sort by |x|, not by value: ascending puts -6 before -3, and -6 looks for -12
// - skip x when count[x] == 0 (an earlier value used it as its double)
// - decrement count[x] first, then look for 2x: for x == 0 the double is the same key
// - check the length is even before anything else
// Tests to write: chain 1 -> 2 -> 4 with a shared 2; negatives; zeros; a missing double;
//   odd length; round trip through (1); (1) keeps the multiset.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;
using std::swap;
using std::abs;
using std::rand;

// (1) append the doubles, then Fisher-Yates: swap each slot with a random slot at or
// before it. T: O(n), S: O(n).
vector<int> doubleAndShuffle(const vector<int>& nums) {
    vector<int> result = nums;
    for (int x : nums) { result.push_back(2 * x); }
    for (int i = (int)result.size() - 1; i > 0; i--) {
        int pick = rand() % (i + 1);
        swap(result[i], result[pick]);
    }
    return result;
}

// (2) sort by |x|: the smallest unused value cannot be anyone's double, so it is an
// original and must consume one copy of 2x. T: O(n log n), S: O(n).
vector<int> recoverOriginal(vector<int> changed) {
    if (changed.size() % 2 != 0) { return {}; }
    sort(changed.begin(), changed.end(), [](int a, int b) { return abs(a) < abs(b); });
    // value -> copies not used yet
    unordered_map<int, int> count;
    for (int x : changed) { count[x]++; }

    vector<int> original;
    for (int x : changed) {
        // this copy was already used as an earlier value's double
        if (count[x] == 0) { continue; }
        count[x]--;
        if (count[2 * x] == 0) { return {}; }
        count[2 * x]--;
        original.push_back(x);
    }
    return original;
}

void test(const string& label, const vector<int>& changed, vector<int> want) {
    vector<int> got = recoverOriginal(changed);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

// (1) is random, so compare multisets: sorted output == sorted nums + doubles
void testShuffle(const string& label, const vector<int>& nums, vector<int> want) {
    vector<int> got = doubleAndShuffle(nums);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {2, 4, 1, 2}, {1, 2});
    test("negative-chain", {-6, -12, -3, -6}, {-3, -6});
    test("zeros", {0, 5, 0, 10}, {0, 5});
    test("missing-double", {1, 2, 3, 5}, {});
    test("odd-length", {1, 2, 4}, {});
    test("round-trip", doubleAndShuffle({3, -1, 0, 5, 3}), {3, -1, 0, 5, 3});
    testShuffle("shuffle-multiset", {3, -1, 0}, {3, -1, 0, 6, -2, 0});
    testShuffle("shuffle-empty", {}, {});
}

// follow-up: only non-negative values (LC 2007) -> plain ascending sort, same greedy
// follow-up: non-negative values up to V -> no sort: a count array of size 2V + 1, walk x
//   upward and pair all copies of x at once, O(n + V):
//     if (count[0] % 2 != 0) { return {}; }
//     original.insert(original.end(), count[0] / 2, 0);
//     for (int x = 1; x <= V; x++) {
//         if (count[2 * x] < count[x]) { return {}; }
//         count[2 * x] -= count[x];
//         original.insert(original.end(), count[x], x);
//     }
// follow-up: multiplier m >= 2 instead of 2 -> the same greedy (|m * x| > |x| still
//   holds); replace the two lookups:
//     if (count[m * x] == 0) { return {}; }
//     count[m * x]--;
// follow-up: only check whether changed is doubled (LC 954) -> the same loop, return
//   false where it returns {} and true at the end
