// LC 136 Single Number (Easy) — XOR cancels pairs
// Every element appears twice except one; return that one in O(n) time and O(1)
// extra space.
// Clarify: exactly one single and the rest exactly twice; negative values; non-empty.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: XOR everything; a ^ a = 0 and 0 ^ x = x in any order, so the pairs
// cancel and the single remains. T: O(n), S: O(1).
int singleNumber(const vector<int>& nums) {
    int single = 0;
    for (int num : nums) {
        single ^= num;
    }
    return single;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (singleNumber(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", {4, 9, 4}, 9);
    test("alone", {7}, 7);
    test("negative", {-3, 5, 5, 8, 8}, -3);
    return 0;
}

// follow-up (ideas only):
// - others appear three times (LC 137) -> per bit, count the set bits mod 3; the
//   leftover bits form the answer, O(32n)
// - two singles (LC 260) -> x = xor of all; split nums by the lowest set bit of x and
//   xor each group
// - majority element (LC 169) -> Boyer-Moore voting: a candidate and its votes, a
//   different value cancels one vote; more than n / 2 copies cannot be cancelled out
// - missing number (LC 268) -> xor of all indices 0..n and all values (or the sum
//   formula n * (n + 1) / 2 - sum)
// - number of 1 bits (LC 191) -> n &= n - 1 clears the lowest set bit, count the steps;
//   reverse bits (LC 190) -> 32 times: shift the result left, take n's low bit, n >>= 1
// - sum of two integers without + (LC 371) -> xor is the sum without carry, (a & b) << 1
//   is the carry; repeat until the carry is 0 (unsigned for the shift)
// - reverse integer (LC 7) -> pop digits with % 10; before result * 10 + digit check
//   against INT_MAX / 10 and INT_MIN / 10
