// LC 238 Product of Array Except Self (Medium) — prefix product * suffix product
// answer[i] = product of all nums except nums[i]
//
// clarify:
// division allowed? -> no (follow-up)
// output array -> not counted as extra space
// zeros / negatives -> ok, no special case w/o division
// overflow -> assume every product fits in int

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: write left products into answer, then multiply in a running right product
// T: O(n), S: O(1) extra
vector<int> productExceptSelf(const vector<int>& nums) {
    int n = nums.size();
    // after pass 1: answer[i] = product of nums[0..i-1]
    vector<int> answer(n, 1);
    for (int i = 1; i < n; i++) {
        answer[i] = answer[i - 1] * nums[i - 1];
    }
    // right = product of nums[i+1..n-1]
    int right = 1;
    for (int i = n - 1; i >= 0; i--) {
        answer[i] *= right;
        right *= nums[i];
    }
    return answer;
}

void test(const string& label, const vector<int>& nums, const vector<int>& want) {
    cout << label << (productExceptSelf(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {2, 3, 4}, {12, 8, 6});
    test("one-zero", {2, 0, 3}, {0, 6, 0});
    test("two-zeros", {0, 2, 0}, {0, 0, 0});
    test("negative", {-1, 2}, {2, -1});
    return 0;
}

// follow-up:
// division allowed -> total / nums[i]; cases by zero cnt: 0, 1 (only that idx != 0), 2+
// clearer O(n) space version -> prefix[i] and suffix[i] arrays, answer[i] = their product
