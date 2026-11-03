// dup count? e.g. [3,3,2], k=2 -> 3
// range of k: 1<=k<=n, always an answer
// allow num modify?
// val neg/dup? yes
// is n huge or stream? stream -> size-k minHeap

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cstdlib>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::rand;
using std::swap;

// randomized quick-select with 3-way partition, dup not stall
// T: O(n) avg, O(n^2) worst, S: O(1)
int findKthLargest(vector<int> nums, int k) {
    int n = nums.size();
    int target = n - k; // sorted asend order
    int lo = 0;
    int hi = n - 1;
    while (lo < hi) {
        int pivot = nums[lo + rand() % (hi - lo + 1)];
        int lt = lo; // [lo, lt) < pivot
        int gt = hi; // (gt, hi] > pivot
        int i = lo; // [lt, i) = pivot, [i, gt] unseen
        while (i <= gt) {
            if (nums[i] < pivot) {
                swap(nums[lt], nums[i]);
                lt++;
                i++;
            } else if (nums[i] > pivot) {
                swap(nums[i], nums[gt]);
                gt--;
            } else {
                i++;
            }
        }

        if (target < lt) { hi = lt - 1; }
        else if (target > gt) { lo = gt + 1; }
        else { return nums[target]; }
    }
    return nums[target];
}

// follow-up: stream input, T: O(nlogk), S: O(k)
int findKthLargestHeap(const vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (int num : nums) {
        minHeap.push(num);
        if ((int)minHeap.size() > k) { minHeap.pop(); }
    }
    return minHeap.top();
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    bool ok = findKthLargest(nums, k) == want && findKthLargestHeap(nums, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {3, 2, 1, 5, 6, 4}, 2, 5);
    test("lc ex2", {3, 2, 3, 1, 2, 4, 5, 5, 6}, 4, 4);
    test("all equal", {7, 7, 7, 7}, 2, 7);
}
