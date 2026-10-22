// LC 179 Largest Number: arrange non-negative integers so that their concatenation is the
// largest possible number, and return it as a string.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::to_string;

// Sort strings by "a before b when a + b > b + a". That relation is a strict weak order
// (it compares a * 10^|b| + b with b * 10^|a| + a, plain integers), so sort is safe.
// T: O(n log n * L) for L-digit numbers, S: O(n * L).
string largestNumber(vector<int>& nums) {
    vector<string> strs;   // decimal text of each number
    for (int x : nums) { strs.push_back(to_string(x)); }
    sort(strs.begin(), strs.end(), [](const string& a, const string& b) {
        return a + b > b + a;
    });
    // all zeros sort to the front, so a leading "0" means the answer is just "0"
    if (strs[0] == "0") { return "0"; }
    string result;
    for (const auto& s : strs) { result += s; }
    return result;
}

void test(const string& label, vector<int> nums, const string& want) {
    cout << label << (largestNumber(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("prefix-order", {8, 89, 898}, "898988");
    test("same-prefix", {12, 121}, "12121");
    test("all-zeros", {0, 0}, "0");
    test("single", {10}, "10");
}
