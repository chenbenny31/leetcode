// LC 912 Sort an Array (Medium) — merge sort / quicksort with a 3-way partition
// Return nums in ascending order without the library sort, in O(n log n).
// Clarify: in place or a new array; stability needed; many duplicates; guaranteed
//   O(n log n) or average is enough; extra memory allowed.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::swap;
using std::rand;

// Sorts nums[lo..hi]; buffer is scratch space of the same size as nums.
void mergeSort(vector<int>& nums, int lo, int hi, vector<int>& buffer) {
    if (lo >= hi) { return; }
    int mid = lo + (hi - lo) / 2;
    mergeSort(nums, lo, mid, buffer);
    mergeSort(nums, mid + 1, hi, buffer);
    int left = lo;
    int right = mid + 1;
    int write = lo;
    while (left <= mid && right <= hi) {
        // <= takes the left half on ties, which keeps the sort stable
        if (nums[left] <= nums[right]) {
            buffer[write] = nums[left];
            left++;
        } else {
            buffer[write] = nums[right];
            right++;
        }
        write++;
    }
    while (left <= mid) {
        buffer[write] = nums[left];
        left++;
        write++;
    }
    while (right <= hi) {
        buffer[write] = nums[right];
        right++;
        write++;
    }
    for (int i = lo; i <= hi; i++) { nums[i] = buffer[i]; }
}

// Core idea: sort each half, then merge the two sorted halves through a buffer with two
// read pointers. T: O(n log n) always, S: O(n).
vector<int> sortArray(vector<int> nums) {
    vector<int> buffer(nums.size());
    mergeSort(nums, 0, (int)nums.size() - 1, buffer);
    return nums;
}

// Sorts nums[lo..hi] in place.
void quickSort(vector<int>& nums, int lo, int hi) {
    if (lo >= hi) { return; }
    int pivot = nums[lo + rand() % (hi - lo + 1)];
    // nums[lo..smaller) < pivot, nums[smaller..cur) == pivot, nums(larger..hi] > pivot
    int smaller = lo;
    int cur = lo;
    int larger = hi;
    while (cur <= larger) {
        if (nums[cur] < pivot) {
            swap(nums[cur], nums[smaller]);
            smaller++;
            cur++;
        } else if (nums[cur] > pivot) {
            // cur stays: the value swapped in is not examined yet
            swap(nums[cur], nums[larger]);
            larger--;
        } else {
            cur++;
        }
    }
    quickSort(nums, lo, smaller - 1);
    quickSort(nums, larger + 1, hi);
}

// Core idea: partition around a random pivot into < / == / > (Dutch flag) and recurse on
// the two outer parts; the == block is final, so all-equal input stays O(n) instead of
// O(n^2). T: O(n log n) average, O(n^2) worst, S: O(log n) average stack.
vector<int> sortArrayQuick(vector<int> nums) {
    quickSort(nums, 0, (int)nums.size() - 1);
    return nums;
}

void test(const string& label, const vector<int>& nums, const vector<int>& want) {
    bool ok = sortArray(nums) == want && sortArrayQuick(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("reversed", {5, 4, 3, 2, 1}, {1, 2, 3, 4, 5});
    test("duplicates-and-negatives", {3, -1, 3, 2, -1, 3}, {-1, -1, 2, 3, 3, 3});
    test("empty", {}, {});
    test("single", {7}, {7});
    return 0;
}

// follow-up (ideas only):
// - why a random pivot -> a fixed first / last pivot on sorted input peels one element
//   per level: O(n^2) time and O(n) stack; random makes that unlikely for any input
// - stability -> merge sort is stable (ties take the left half); quicksort is not, its
//   long-distance swaps reorder equal keys
// - k-th largest (LC 215) -> quickselect: same partition, continue only into the side
//   that holds index n - k; O(n) average
// - count inversions / reverse pairs (LC 493) -> count during the merge: when the right
//   element is taken, every element left in the left half forms a pair with it
// - sort a linked list (LC 148) -> merge sort: split with slow / fast, merge by
//   relinking; no random access needed
// - heap sort -> build a max-heap in place, swap the top to the end and sift down:
//   O(n log n) worst case, O(1) space, not stable
// - data larger than memory (external sort) -> sort memory-sized chunks to files, then a
//   k-way merge with a min-heap of each chunk's next value
