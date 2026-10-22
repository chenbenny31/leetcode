// LC 137 Single Number II: every element appears three times except one; return that
// one in linear time and constant extra space.
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::string;
using std::vector;

// Per bit position, the triples contribute a multiple of 3 to the count of set bits, so
// count % 3 is the answer's bit. Unsigned shifts avoid the signed-shift trap.
// T: O(32*n), S: O(1).
int singleNumber(vector<int>& nums) {
    unsigned res = 0;
    for (int i = 0; i < 32; i++) {
        int cnt = 0;
        for (int x : nums) { cnt += (unsigned(x) >> i) & 1; }
        if (cnt % 3 != 0) { res |= 1u << i; }
    }
    return int(res);
}

void test(const string& label, vector<int> nums, int want) {
    cout << label << (singleNumber(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", {5, 2, 5, 5}, 2);
    test("negative", {-4, 6, 6, 6}, -4);
    test("alone", {9}, 9);
    test("negative-triples", {-1, -1, -1, 3}, 3);
}
