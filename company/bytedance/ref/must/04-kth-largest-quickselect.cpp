// ByteDance (hack2hire) / LC 215: return the k-th largest element of nums (k-th in
// sorted order, duplicates count). No heap and no full sort.
// Follow-up: size-k min-heap version.

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

// Randomized quickselect, 3-way partition so duplicates never stall it.
// T: O(n) average, O(n^2) worst, S: O(1) extra (nums is a copy).
int findKthLargest(vector<int> nums, int k) {
    int n = nums.size();
    // index of the answer if nums were sorted ascending
    int target = n - k;
    // the answer stays inside [lo, hi]
    int lo = 0;
    int hi = n - 1;
    while (lo < hi) {
        // random pivot each round: no fixed input forces O(n^2)
        int pivot = nums[lo + rand() % (hi - lo + 1)];
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
    // lo == hi == target: a one-element range is already in place
    return nums[target];
}

// Follow-up: keep the k largest seen so far; their min is the answer.
// T: O(n log k), S: O(k).
int findKthLargestHeap(const vector<int>& nums, int k) {
    // k largest so far, min on top
    priority_queue<int, vector<int>, greater<int>> heap;
    for (int x : nums) {
        heap.push(x);
        if ((int)heap.size() > k) { heap.pop(); }
    }
    return heap.top();
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    bool ok = findKthLargest(nums, k) == want && findKthLargestHeap(nums, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("distinct", {3, 2, 1, 5, 6, 4}, 2, 5);
    test("dups", {3, 2, 3, 1, 2, 4, 5, 5, 6}, 4, 4);
    test("all-equal", {7, 7, 7, 7}, 2, 7);
    test("single", {5}, 1, 5);
}

// follow-up: stream / too big for memory -> see findKthLargestHeap() above; size-k
//   min-heap, O(n log k) time, O(k) memory
// follow-up: worst-case O(n) -> median-of-medians pivot (groups of 5); in practice
//   nth_element(nums.begin(), nums.begin() + n - k, nums.end()) (introselect)
// follow-up: top k elements -> after quickselect nums[n-k, n) holds them; sort if order
//   is needed, O(n + k log k)
// follow-up: small value range R -> count[v - minV], scan from max down until k reached,
//   O(n + R)
