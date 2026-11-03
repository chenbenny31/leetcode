// ByteDance (FastPrep reconstruction, ~20% match: the report only said "a NumPy data
// transformation"): for each element of values apply every transform [a, b] in order,
// x -> a * x + b, and return the final values in the original order. No transforms ->
// unchanged; a = 0 resets the value to b and later transforms still apply. Exact
// 64-bit arithmetic (|values[i]| <= 1e6, <= 20 transforms, -2 <= a <= 2, |b| <= 1000).

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
