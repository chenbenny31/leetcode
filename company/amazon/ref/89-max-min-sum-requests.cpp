// For each of n requests: append (current max + current min) of arr to the answer, then
// decrease one occurrence of the max by 1 (it stays in the array). Return the answers.

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <iterator>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::multiset;
using std::prev;
using std::max_element;
using std::min_element;

// straightforward: scan for max and min every time. T: O(n * size), S: O(1) extra.
vector<long long> maxMinSums(vector<int> arr, int n) {
    vector<long long> result;
    for (int q = 0; q < n; q++) {
        auto hi = max_element(arr.begin(), arr.end());
        auto lo = min_element(arr.begin(), arr.end());
        result.push_back((long long)*hi + *lo);
        (*hi)--;
    }
    return result;
}

// improved: a multiset keeps the values sorted with duplicates, so min and max are its
// ends; replace one max by max - 1. T: O(size log size + n log size), S: O(size).
vector<long long> maxMinSumsSet(const vector<int>& arr, int n) {
    multiset<int> values(arr.begin(), arr.end());
    vector<long long> result;
    for (int q = 0; q < n; q++) {
        int hi = *values.rbegin(), lo = *values.begin();
        result.push_back((long long)hi + lo);
        values.erase(prev(values.end()));
        values.insert(hi - 1);
    }
    return result;
}

void test(const string& label, const vector<int>& arr, int n,
          const vector<long long>& want) {
    bool ok = maxMinSums(arr, n) == want && maxMinSumsSet(arr, n) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("spread", {1, 4, 6}, 3, {7, 6, 5});
    test("dups", {3, 3}, 2, {6, 5});
    test("single", {5}, 2, {10, 8});
    test("max-becomes-min", {1, 1}, 2, {2, 1});
}
