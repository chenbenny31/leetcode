// LC 349: return the distinct values present in both arrays (any order; sorted here).

#include <iostream>
#include <vector>
#include <string>
#include <set>

using std::cout;
using std::string;
using std::vector;
using std::set;

// put nums1 in a set, keep the nums2 values found in it. T: O((n + m) log n), S: O(n).
vector<int> intersection(const vector<int>& nums1, const vector<int>& nums2) {
    set<int> first(nums1.begin(), nums1.end()), both;
    for (int x : nums2) {
        if (first.count(x)) { both.insert(x); }
    }
    return vector<int>(both.begin(), both.end());
}

void test(const string& label, const vector<int>& a, const vector<int>& b,
          const vector<int>& want) {
    cout << label << (intersection(a, b) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dups", {1, 2, 2, 3}, {2, 2, 3, 3}, {2, 3});
    test("none", {1}, {2}, {});
    test("same", {5, 4}, {4, 5}, {4, 5});
}
