// LC 496 Next Greater Element I: nums1 is a subset of nums2 (distinct values). For each
// x in nums1 return the first greater value to the right of x in nums2, or -1.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// Monotonic decreasing stack over nums2 resolves every value once; lookup per query.
// T: O(n1 + n2), S: O(n2).
vector<int> nextGreaterElement(const vector<int>& nums1, const vector<int>& nums2) {
    unordered_map<int, int> greater;   // value of nums2 -> its next greater value
    vector<int> stk;                   // values still waiting, decreasing bottom to top
    for (int x : nums2) {
        while (!stk.empty() && stk.back() < x) {
            greater[stk.back()] = x;
            stk.pop_back();
        }
        stk.push_back(x);
    }
    vector<int> res;
    for (int x : nums1) {
        auto it = greater.find(x);
        if (it == greater.end()) {
            res.push_back(-1);
        } else {
            res.push_back(it->second);
        }
    }
    return res;
}

void test(const string& label, const vector<int>& nums1, const vector<int>& nums2,
          const vector<int>& want) {
    if (nextGreaterElement(nums1, nums2) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("found-and-last", {2, 4}, {1, 2, 3, 4}, {3, -1});
    test("same-greater", {3, 1}, {3, 1, 4}, {4, 4});
    test("decreasing", {4, 3}, {4, 3, 2}, {-1, -1});
    test("empty-query", {}, {1}, {});
}
