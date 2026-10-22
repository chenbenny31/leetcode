// LC 215 Kth Largest Element in an Array: return the kth largest value of nums
// (kth in sorted order, duplicates counted).
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;

// Min-heap capped at k: it keeps the k largest seen, so its top is the kth largest.
// T: O(n log k), S: O(k).
int findKthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> heap;   // k largest values so far
    for (int x : nums) {
        heap.push(x);
        if ((int)heap.size() > k) { heap.pop(); }
    }
    return heap.top();
}

void test(const string& label, vector<int> nums, int k, int want) {
    cout << label << (findKthLargest(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", {4, 9, 1, 7}, 2, 7);
    test("duplicates", {5, 5, 5, 2}, 3, 5);
    test("k-equals-n", {3, -1, 8}, 3, -1);
    test("single", {6}, 1, 6);
}
