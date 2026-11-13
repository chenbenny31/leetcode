// 91 · Kth Largest Element in an Array — LC 215, 462, 912
// Mother problem: select/10-heap/01-kth-largest.cpp
//
// LC 215 (Medium), k-min-heap / quickselect: return the k-th largest element of nums
// (k-th in sorted order, duplicates count: {5, 5, 1, 3}, k = 3 → 3). 1 <= k <= n.
// Google reports: median moves, merge sort, insert + k-th largest, implement quicksort
//
// Core idea
// - Size-k min-heap of the largest so far; its top is the answer: O(n log k), S O(k).
// - Quickselect with a random pivot and 3-way partition (duplicates never stall it):
//   O(n) average, O(n^2) worst; works on a copy.
//
// Tests: distinct, duplicates, all equal, negatives
//
// Follow-ups
// - min moves to make all equal (LC 462) → sum of |x - median| in long long, median =
//   quickSelect(nums, n - n / 2)
// - sort an array (LC 912) → `sortArray` below (merge sort, stable, O(n log n))
// - implement quicksort → `quickSort` below (Lomuto, random pivot)
// - worst-case O(n) → median-of-medians pivot; in practice nth_element (introselect)
// - top k elements → after quickselect nums[n-k, n) holds them, O(n)
// - increment-only moves (LC 3736) → raise everything to the max: max * n - sum
// - last stone weight (LC 1046) → max-heap: pop the two heaviest, push the nonzero diff
//
//////////////////////////////////////////////////////////////////////////////////////////

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

// Keep the k largest seen so far; their min is the answer. T: O(n log k), S: O(k).
int findKthLargest(const vector<int>& nums, int k) {
    // k largest so far, smallest on top
    priority_queue<int, vector<int>, greater<int>> heap;
    for (int num : nums) {
        heap.push(num);
        if ((int)heap.size() > k) { heap.pop(); }
    }
    return heap.top();
}

// Randomized quickselect with 3-way partition (duplicates never stall it).
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
    // lo == hi == target: a one-element range is already in place
    return nums[target];
}

// sorts nums[lo..hi] (inclusive); buffer is scratch space as long as nums
void mergeSort(vector<int>& nums, int lo, int hi, vector<int>& buffer) {
    if (lo >= hi) {
        return;
    }
    int mid = lo + (hi - lo) / 2;
    mergeSort(nums, lo, mid, buffer);
    mergeSort(nums, mid + 1, hi, buffer);
    // merge sorted [lo, mid] and [mid + 1, hi] into buffer[lo..hi]
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

// LC 912: merge sort, O(n log n) on every input. T: O(n log n), S: O(n).
vector<int> sortArray(vector<int> nums) {
    vector<int> buffer(nums.size());
    mergeSort(nums, 0, (int)nums.size() - 1, buffer);
    return nums;
}

// Lomuto partition of nums[lo..hi] around a random pivot; returns the pivot's final
// index, with smaller values left of it and the rest right of it
int partitionRange(vector<int>& nums, int lo, int hi) {
    int pivotIndex = lo + rand() % (hi - lo + 1);
    // park the pivot at hi so the scan never moves it
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

// Google "implement quicksort": random pivot, partition, recurse on both sides. A run of
// equal values still goes O(n^2) with Lomuto (quickSelect's 3-way partition fixes it).
// T: O(n log n) average, O(n^2) worst, S: O(log n) average recursion.
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
