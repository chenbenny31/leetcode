// LC 912 Sort an Array (Medium) — merge sort / quicksort with a 3-way partition
// return nums ascending w/o library sort, in O(n log n)
//
// clarify:
// in place or new array? -> return a sorted copy here
// stability needed? -> merge sort
// many dups? -> quicksort needs the 3-way partition
// guaranteed O(n log n) or average? -> guaranteed = merge sort
// extra memory? -> merge sort needs an O(n) buffer

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

// sorts nums[lo..hi]; buffer = scratch space, same size as nums
void mergeSort(vector<int>& nums, int lo, int hi, vector<int>& buffer) {
    if (lo >= hi) { return; }
    int mid = lo + (hi - lo) / 2;
    mergeSort(nums, lo, mid, buffer);
    mergeSort(nums, mid + 1, hi, buffer);
    int left = lo;
    int right = mid + 1;
    int write = lo;
    while (left <= mid && right <= hi) {
        // <= takes the left half on ties -> stable
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

// solution 1: merge sort; stable, guaranteed O(n log n)
// core idea: sort each half, merge the two sorted halves through a buffer
// T: O(n log n), S: O(n)
vector<int> sortArray(vector<int> nums) {
    vector<int> buffer(nums.size());
    mergeSort(nums, 0, (int)nums.size() - 1, buffer);
    return nums;
}

// sorts nums[lo..hi] in place
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

// solution 2: quicksort w/ 3-way partition; in place, not stable
// core idea: random pivot, split into < / == / > (Dutch flag), recurse on outer parts
// == block is final, so all-equal input stays O(n)
// T: O(n log n), S: O(log n) — average; worst O(n^2) time, O(n) stack
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

// follow-up:
// why random pivot -> fixed pivot on sorted input: O(n^2) time, O(n) stack
// stability -> merge sort stable (ties take left); quicksort not (long-distance swaps)
// k-th largest (LC 215) -> quickselect: continue only into side w/ idx n - k; O(n) avg
// cnt inversions / reverse pairs (LC 493) -> count during merge when right value is taken
// sort a linked list (LC 148) -> merge sort: split w/ slow / fast, merge by relinking
// heap sort -> max-heap in place, swap top to end, sift down; O(1) space, not stable
// external sort -> sort memory-sized chunks to files, k-way merge w/ min-heap
