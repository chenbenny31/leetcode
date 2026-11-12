// Double and Shuffle / Recover Original (Google, interview report), two parts.
// (1) doubleAndShuffle(nums): nums followed by 2 * each element, uniformly shuffled.
// (2) recoverOriginal(changed): return any array a such that a + its doubles, shuffled,
//     is changed (listed in ascending |value| order); empty if impossible. Values may be
//     negative; zeros pair with zeros; doubled values are assumed to fit in int.

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

int main() {
    test("chain", {2, 4, 1, 2}, {1, 2});
    test("negative-chain", {-6, -12, -3, -6}, {-3, -6});
    test("zeros", {0, 5, 0, 10}, {0, 5});
    test("missing-double", {1, 2, 3, 5}, {});
    test("round-trip", doubleAndShuffle({3, -1, 0, 5, 3}), {3, -1, 0, 5, 3});
}

// follow-up: only non-negative values (LC 2007) -> plain ascending sort, same greedy
// follow-up: values bounded by V -> counting array instead of map, O(n + V)
