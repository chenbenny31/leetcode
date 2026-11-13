// LC 215 Kth Largest Element in an Array (Medium) — size-k min-heap / quickselect
// Return the k-th largest element of nums: the k-th in descending sorted order, so
// duplicates count.
// Clarify: 1 <= k <= n; duplicates count separately; may nums be modified; average or
//   worst-case O(n) wanted.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <cstdlib>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::rand;
using std::swap;

// Core idea: keep the k largest seen so far in a min-heap; its top is the k-th largest.
// T: O(n log k), S: O(k).
int findKthLargest(const vector<int>& nums, int k) {
    // k largest so far, smallest on top
    priority_queue<int, vector<int>, greater<int>> heap;
    for (int num : nums) {
        heap.push(num);
        if ((int)heap.size() > k) { heap.pop(); }
    }
    return heap.top();
}

// Core idea: quickselect with a random pivot and a 3-way partition (duplicates never
// stall it); continue only in the part that holds ascending index n - k.
// T: O(n) average, O(n^2) worst, S: O(1) extra (nums is a copy).
int quickSelect(vector<int> nums, int k) {
    int n = nums.size();
    // index of the answer if nums were sorted ascending
    int target = n - k;
    // the answer stays inside [lo, hi]
    int lo = 0;
    int hi = n - 1;
    while (lo < hi) {
        int pivotIndex = lo + rand() % (hi - lo + 1);
        int pivot = nums[pivotIndex];
        // [lo, lt) < pivot, [lt, i) == pivot, [i, gt] unseen, (gt, hi] > pivot
        int lt = lo;
        int i = lo;
        int gt = hi;
        while (i <= gt) {
            if (nums[i] < pivot) {
                swap(nums[lt], nums[i]);
                lt++;
                i++;
            } else if (nums[i] > pivot) {
                // i stays: the value swapped in from gt is not classified yet
                swap(nums[i], nums[gt]);
                gt--;
            } else {
                i++;
            }
        }
        if (target < lt) {
            hi = lt - 1;
        } else if (target > gt) {
            lo = gt + 1;
        } else {
            return nums[target];
        }
    }
    return nums[target];
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    bool ok = findKthLargest(nums, k) == want && quickSelect(nums, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("distinct", {7, 1, 5, 3}, 2, 5);
    test("dups", {5, 5, 1, 3}, 3, 3);
    test("all-equal", {7, 7, 7, 7}, 2, 7);
    test("negatives", {-1, -5, -3}, 3, -5);
    return 0;
}

// follow-up (ideas only):
// - sort an array (LC 912) -> 02-two-pointers/10-sort-an-array
// - worst-case O(n) -> median-of-medians pivot (groups of 5); in practice nth_element
//   (introselect)
// - the top k elements, not only the k-th -> after quickselect nums[n-k, n) holds them
// - min +-1 moves to make all elements equal (LC 462) -> move everything to the median
//   (quickselect with k = n - n / 2) and sum |x - median| in long long
// - increment-only moves to equal (LC 3736) -> raise everything to the max:
//   max * n - sum in long long
// - last stone weight (LC 1046) -> max-heap simulation: pop the two heaviest, push
//   their difference if it is nonzero
// - k-th largest in a stream (LC 703) -> keep the size-k min-heap alive between adds
