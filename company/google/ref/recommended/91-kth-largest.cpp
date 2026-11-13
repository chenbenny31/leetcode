// 91 · Kth Largest Element in an Array — LC 215, 462, 912
// Mother: select/10-heap/01-kth-largest.cpp
// (1) k-th largest of nums (k-th in sorted order, dup count), 1 <= k <= n
// (2) sort an array w/ merge sort (LC 912)
// (3) implement quicksort
// reported: median moves, merge sort, insert + k-th largest, implement quicksort
//
// clarify:
// dup -> take separate ranks
// may modify nums -> not needed, quickselect works on a copy
// sort must be stable? -> merge sort is, quicksort is not
// average vs worst-case time -> quickselect / quicksort O(n^2) worst, random pivot

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

// (1) k-th largest
// solution 1: min-heap of size k; also works on a stream (insert + k-th largest)
// core idea: keep the k largest seen so far in a min-heap; top = answer
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

// (1) k-th largest
// solution 2: quickselect; O(n) average when all of nums is in memory
// core idea: 3-way partition around a random pivot (dup never stall it)
// then keep only the side holding idx n - k
// T: O(n), S: O(1) — average; O(n^2) worst; S beyond the copy of nums
int quickSelect(vector<int> nums, int k) {
    int n = nums.size();
    // idx of the answer if nums were sorted ascending
    int target = n - k;
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

// sorts nums[lo..hi] (inclusive); buffer = scratch space as long as nums
void mergeSort(vector<int>& nums, int lo, int hi, vector<int>& buffer) {
    if (lo >= hi) {
        return;
    }
    int mid = lo + (hi - lo) / 2;
    mergeSort(nums, lo, mid, buffer);
    mergeSort(nums, mid + 1, hi, buffer);
    int left = lo;
    int right = mid + 1;
    int pos = lo;
    while (left <= mid && right <= hi) {
        // <= takes the left one on ties: stable
        if (nums[left] <= nums[right]) {
            buffer[pos] = nums[left];
            left++;
        } else {
            buffer[pos] = nums[right];
            right++;
        }
        pos++;
    }
    while (left <= mid) {
        buffer[pos] = nums[left];
        left++;
        pos++;
    }
    while (right <= hi) {
        buffer[pos] = nums[right];
        right++;
        pos++;
    }
    for (int i = lo; i <= hi; i++) {
        nums[i] = buffer[i];
    }
}

// (2) merge sort
// core idea: sort each half, then merge the two sorted halves through a buffer
// T: O(n log n), S: O(n)
vector<int> sortArray(vector<int> nums) {
    vector<int> buffer(nums.size());
    mergeSort(nums, 0, (int)nums.size() - 1, buffer);
    return nums;
}

// Lomuto partition of nums[lo..hi] on a random pivot; returns its final idx
int partitionRange(vector<int>& nums, int lo, int hi) {
    int pivotIndex = lo + rand() % (hi - lo + 1);
    // park pivot at hi so the scan never moves it
    swap(nums[pivotIndex], nums[hi]);
    int pivot = nums[hi];
    // nums[lo, write) < pivot
    int write = lo;
    for (int read = lo; read < hi; read++) {
        if (nums[read] < pivot) {
            swap(nums[write], nums[read]);
            write++;
        }
    }
    swap(nums[write], nums[hi]);
    return write;
}

// sorts nums[lo..hi] (inclusive)
void quickSortRange(vector<int>& nums, int lo, int hi) {
    if (lo >= hi) {
        return;
    }
    int mid = partitionRange(nums, lo, hi);
    quickSortRange(nums, lo, mid - 1);
    quickSortRange(nums, mid + 1, hi);
}

// (3) quicksort
// core idea: random pivot, Lomuto partition, recurse on both sides
// a run of equal values still goes O(n^2); 3-way partition fixes it
// T: O(n log n), S: O(log n) — average; O(n^2) time worst
vector<int> quickSort(vector<int> nums) {
    quickSortRange(nums, 0, (int)nums.size() - 1);
    return nums;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    bool ok = findKthLargest(nums, k) == want && quickSelect(nums, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testSort(const string& label, const vector<int>& nums, const vector<int>& want) {
    bool ok = sortArray(nums) == want && quickSort(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("distinct", {7, 1, 5, 3}, 2, 5);
    test("dups", {5, 5, 1, 3}, 3, 3);
    test("all-equal", {7, 7, 7, 7}, 2, 7);
    test("negatives", {-1, -5, -3}, 3, -5);
    testSort("sort-reversed", {5, 4, 3, 2, 1}, {1, 2, 3, 4, 5});
    testSort("sort-dups-negatives", {3, -1, 3, 0, -1}, {-1, -1, 0, 3, 3});
    testSort("sort-one", {7}, {7});
    testSort("sort-empty", {}, {});
}

// follow-up:
// min moves to make all equal (LC 462, Google) -> move all to median, sum |x - median|
// LC 462 details -> sum in long long, median by quickselect
// worst-case O(n) selection -> median-of-medians pivot; in practice nth_element
// top k elements -> after quickselect the last k slots hold them, O(n)
// increment-only moves (LC 3736) -> raise everything to the max: max * n - sum
// last stone weight (LC 1046) -> max-heap: pop the two heaviest, push nonzero difference
