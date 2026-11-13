// LC 136 Single Number (Easy) — XOR cancels pairs
// every num appears twice except one; return it in O(n) time, O(1) space
//
// clarify:
// exactly one single, rest exactly twice -> plain XOR works
// negative values -> ok, XOR works on any bits
// empty input -> assume non-empty
// rest appear 3 times? -> follow-up (LC 137)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: XOR all; a ^ a = 0, 0 ^ x = x in any order -> pairs cancel
// T: O(n), S: O(1)
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

// follow-up:
// others appear 3 times (LC 137) -> per bit, cnt set bits mod 3; O(32n)
// two singles (LC 260) -> x = xor of all; split by lowest set bit of x, xor each group
// majority element (LC 169) -> Boyer-Moore voting: candidate + votes, other value cancels
// missing number (LC 268) -> xor of idx 0..n and all values (or n * (n + 1) / 2 - sum)
// number of 1 bits (LC 191) -> n &= n - 1 clears lowest set bit, cnt the steps
// reverse bits (LC 190) -> 32 times: result <<= 1, add n's low bit, n >>= 1
// sum w/o + (LC 371) -> xor = sum w/o carry, (a & b) << 1 = carry; repeat until carry 0
// reverse integer (LC 7) -> pop digits w/ % 10; check INT_MAX / 10 before result * 10
