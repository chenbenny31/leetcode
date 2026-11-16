// Double and Shuffle / Recover Original (Google, interview report), two parts.
// (1) doubleAndShuffle(nums): nums + 2 * each element, uniformly shuffled
// (2) recoverOriginal(changed): any a w/ a + its doubles, shuffled = changed; {} if none
//
// clarify:
// uniform -> every order equally likely
// odd len -> impossible, return {}
// negatives / zeros -> allowed; sort by |x|, zeros pair w/ zeros
// output order -> any valid a; here ascending |value|
// 2 * x overflow -> assume it fits in int

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

// (1) double and shuffle
// core idea: append doubles, then Fisher-Yates: swap slot i w/ a random slot in [0, i]
// T: O(n), S: O(n)
vector<int> doubleAndShuffle(const vector<int>& nums) {
    vector<int> result = nums;
    for (int x : nums) { result.push_back(2 * x); }
    for (int i = (int)result.size() - 1; i > 0; i--) {
        int pick = rand() % (i + 1);
        swap(result[i], result[pick]);
    }
    return result;
}

// (2) recover original
// core idea: sort by |x|; smallest unused value is nobody's double -> it is an original
// each original consumes one copy of 2x
// T: O(n log n), S: O(n)
vector<int> recoverOriginal(vector<int> changed) {
    if (changed.size() % 2 != 0) { return {}; }
    sort(changed.begin(), changed.end(), [](int a, int b) { return abs(a) < abs(b); });
    // value -> copies not used yet
    unordered_map<int, int> count;
    for (int x : changed) { count[x]++; }

    vector<int> original;
    for (int x : changed) {
        // this copy already used as an earlier value's double
        if (count[x] == 0) { continue; }
        // take x before looking for 2x: for x == 0 both are the same key
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

// (1) is random -> compare multisets: sorted output == sorted nums + doubles
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
    testShuffle("shuffle-multiset", {3, -1, 0}, {3, -1, 0, 6, -2, 0});
    testShuffle("shuffle-empty", {}, {});
}

// follow-up:
// why swap w/ a slot in [0, i] only -> picking from the whole array each time is biased
// rand() % m -> slightly biased too; real code: mt19937 + uniform_int_distribution
// only non-negative values (LC 2007) -> plain ascending sort, same greedy
// non-negative values up to V -> cnt array of size 2V + 1, walk x upward, O(n + V)
// multiplier m >= 2 instead of 2 -> same greedy looking up m * x (|m * x| > |x| holds)
// only check whether changed is doubled (LC 954) -> same loop, false for {}, true at end
