// LC 503 Next Greater Element II: circular array nums. For each element return the first
// greater value found moving right and wrapping around, or -1.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Monotonic decreasing stack over the array walked twice (indices mod n). Only the first
// pass pushes; the second pass resolves what is left. T: O(n), S: O(n).
vector<int> nextGreaterElements(const vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, -1);
    vector<int> stk;   // indices still waiting, values decreasing bottom to top
    for (int i = 0; i < 2 * n; i++) {
        int x = nums[i % n];
        while (!stk.empty() && nums[stk.back()] < x) {
            res[stk.back()] = x;
            stk.pop_back();
        }
        if (i < n) { stk.push_back(i); }
    }
    return res;
}

void test(const string& label, const vector<int>& nums, const vector<int>& want) {
    if (nextGreaterElements(nums) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("wrap-around", {2, 1, 3, 1}, {3, 3, -1, 2});
    test("equal-values", {5, 5}, {-1, -1});
    test("single", {7}, {-1});
    test("empty", {}, {});
}
