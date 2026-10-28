// Return nums sorted so that more frequent values come first (all copies together);
// equal frequencies go by smaller value first.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;

// count, then sort by (frequency desc, value asc). T: O(n log n), S: O(n).
vector<int> sortByFrequency(vector<int> nums) {
    // freq[x] = occurrences of x
    unordered_map<int, int> freq;
    for (int x : nums) { freq[x]++; }
    sort(nums.begin(), nums.end(), [&](int a, int b) {
        if (freq[a] != freq[b]) { return freq[a] > freq[b]; }
        return a < b;
    });
    return nums;
}

void test(const string& label, const vector<int>& nums, const vector<int>& want) {
    cout << label << (sortByFrequency(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {3, 1, 3, 2, 3, 1}, {3, 3, 3, 1, 1, 2});
    test("tie-value", {9, 4, 9, 4}, {4, 4, 9, 9});
    test("all-once", {2, 1}, {1, 2});
    test("single", {7}, {7});
}
