// LC 315 Count of Smaller Numbers After Self: for each i, count the j > i with
// nums[j] < nums[i]. Values are in [-10^4, 10^4].
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Scan right to left with a Fenwick tree of value counts; the answer for nums[i] is the
// number of already-seen values strictly smaller. T: O(n log V), S: O(V).
vector<int> countSmaller(vector<int>& nums) {
    int size = 20001;                  // values shifted by +10001 land in 1..20001
    // Fenwick tree over shifted values: tree[k] sums a block of counts ending at k
    vector<int> tree(size + 1, 0);
    int n = nums.size();
    vector<int> result(n, 0);
    for (int i = n - 1; i >= 0; i--) {
        int idx = nums[i] + 10000;     // prefix up to idx covers values < nums[i]
        for (int k = idx; k > 0; k -= k & -k) { result[i] += tree[k]; }
        for (int k = idx + 1; k <= size; k += k & -k) { tree[k]++; }
    }
    return result;
}

void test(const string& label, vector<int> nums, const vector<int>& want) {
    cout << label << (countSmaller(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {4, 1, 3, 2}, {3, 0, 1, 0});
    test("duplicates", {2, 2, 2}, {0, 0, 0});
    test("negatives-extremes", {10000, -10000, 0}, {2, 0, 0});
    test("single", {7}, {0});
}
