// ByteDance: return the value that occurs most often in nums (non-empty); when several
// values share the highest count, return the smallest of them.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// Count, then scan the counts keeping (highest count, smallest value). T: O(n), S: O(n).
int mostFrequentInteger(const vector<int>& nums) {
    // count[v] = occurrences of v
    unordered_map<int, int> count;
    for (int x : nums) {
        count[x]++;
    }

    int best = nums[0];
    int bestCount = 0;
    for (auto& [v, c] : count) {
        if (c > bestCount || (c == bestCount && v < best)) {
            best = v;
            bestCount = c;
        }
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (mostFrequentInteger(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {7}, 7);
    test("tie-smaller", {3, 1, 3, 1}, 1);
    test("negative", {-2, -2, 5}, -2);
    test("clear-winner", {9, 8, 8, 9, 9}, 9);
}
