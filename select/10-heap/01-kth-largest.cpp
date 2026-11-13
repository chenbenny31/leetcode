// LC 215 Kth Largest Element in an Array (Medium) — size-k min-heap / quickselect
// return k-th largest of nums = k-th in descending sorted order (dup count)
//
// clarify:
// k range -> 1 <= k <= n
// dup -> count separately
// modify nums? -> quickselect reorders; here it works on a by-value copy
// avg or worst-case O(n)? -> quickselect is avg; worst-case = median of medians

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

// solution 1: size-k min-heap; simple, works on a stream
// core idea: keep k largest so far in a min-heap; top = k-th largest
// T: O(n log k), S: O(k)
int findKthLargest(const vector<int>& nums, int k) {
    // k largest so far, smallest on top
    priority_queue<int, vector<int>, greater<int>> heap;
    for (int num : nums) {
        heap.push(num);
        if ((int)heap.size() > k) { heap.pop(); }
    }
    return heap.top();
}

// solution 2: quickselect; avg O(n), needs the whole array
// core idea: random pivot + 3-way partition; continue only in the part w/ idx n - k
// T: O(n), S: O(1) — avg, worst O(n^2); space besides the by-value copy
int quickSelect(vector<int> nums, int k) {
    int n = nums.size();
    // idx of the answer if nums were sorted ascending
    int target = n - k;
    // answer stays inside [lo, hi]
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
                // i stays: value swapped in from gt is not classified yet
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

// follow-up:
// sort an array (LC 912) -> 02-two-pointers/10-sort-an-array
// worst-case O(n) -> median-of-medians pivot (groups of 5); in practice nth_element
// top k elements, not only k-th -> after quickselect nums[n-k, n) holds them
// min +-1 moves to equal (LC 462) -> move all to median (quickselect), sum |x - median|
// increment-only moves to equal (LC 3736) -> raise all to max: max * n - sum, long long
// last stone weight (LC 1046) -> max-heap: pop two heaviest, push diff if nonzero
// k-th largest in a stream (LC 703) -> keep the size-k min-heap alive between adds
