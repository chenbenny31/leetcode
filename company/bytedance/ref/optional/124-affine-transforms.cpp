// ByteDance (FastPrep reconstruction, ~20% match): apply every transform [a, b] in order,
// x -> a * x + b, to each value; return the results in order (a = 0 resets x to b).
// Exact 64-bit math: |values[i]| <= 1e6, <= 20 transforms, |a| <= 2, |b| <= 1000.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Apply the transforms to each value in turn. T: O(n * t), S: O(n).
vector<long long> transformArray(const vector<int>& values,
                                 const vector<vector<int>>& transforms) {
    vector<long long> result;
    for (int v : values) {
        long long x = v;
        for (const vector<int>& t : transforms) {
            x = t[0] * x + t[1];
        }
        result.push_back(x);
    }
    return result;
}

void test(const string& label, const vector<int>& values,
          const vector<vector<int>>& transforms, const vector<long long>& want) {
    cout << label << (transformArray(values, transforms) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("scale-shift", {3}, {{2, 0}, {1, -1}}, {5});
    test("reset", {5, -5}, {{0, 9}}, {9, 9});
    test("empty", {}, {}, {});
    test("negative-a", {1}, {{-2, 1}, {-2, 1}}, {3});
}

// follow-up: many values, long transform list -> compose first: (a2, b2) after (a1, b1)
//   = (a2 * a1, a2 * b1 + b2); apply once per value, O(n + t)
// follow-up: overflow bound -> |x'| <= 2|x| + 1000 per step, 20 steps ~ 1e12, fits in
//   long long; larger -> __int128 or work mod p
// follow-up: transforms on index ranges + point queries -> segment tree with lazy (a, b)
//   tags composed as above, O(log n) per op
