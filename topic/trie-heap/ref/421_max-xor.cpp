// LC 421 Maximum XOR of Two Numbers in an Array: given non-negative nums, return the
// maximum nums[i] XOR nums[j] over all pairs (i may equal j).
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

struct BitNode {
    int child[2] = {};   // child[b] = pool index of the next node for bit b, 0 = none
};

// Bit trie over the 31 low bits, high to low. For each number walk the trie taking the
// opposite bit whenever it exists, which greedily maximizes the XOR from the top bit.
// T: O(31 * n), S: O(31 * n).
int findMaximumXOR(vector<int>& nums) {
    vector<BitNode> pool(1);   // node 0 is the root
    for (int x : nums) {
        int cur = 0;
        for (int i = 30; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if (pool[cur].child[bit] == 0) {
                // push_back may reallocate, so take the index before using pool[cur]
                int fresh = pool.size();
                pool.push_back(BitNode());
                pool[cur].child[bit] = fresh;
            }
            cur = pool[cur].child[bit];
        }
    }
    int best = 0;
    for (int x : nums) {
        int cur = 0;
        int xored = 0;
        for (int i = 30; i >= 0; i--) {
            int want = 1 - ((x >> i) & 1);
            if (pool[cur].child[want] != 0) {
                xored |= 1 << i;
                cur = pool[cur].child[want];
            } else {
                cur = pool[cur].child[1 - want];
            }
        }
        best = max(best, xored);
    }
    return best;
}

void test(const string& label, vector<int> nums, int want) {
    cout << label << (findMaximumXOR(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("pair", {1, 2, 4}, 6);
    test("single", {7}, 0);
    test("equal", {5, 5}, 0);
    test("large-bit", {1, 1073741824, 3}, 1073741827);
}
