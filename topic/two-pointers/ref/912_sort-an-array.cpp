// LC 912 Sort an Array: sort ascending in O(n log n) without built-in sort functions.
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;
using std::rand;

// sort nums[lo, hi): sort both halves, then merge them through buf
// T: O(n log n), S: O(log n) stack besides buf.
void mergeSort(vector<int>& nums, vector<int>& buf, int lo, int hi) {
    if (hi - lo <= 1) { return; }
    int mid = lo + (hi - lo) / 2;
    mergeSort(nums, buf, lo, mid);
    mergeSort(nums, buf, mid, hi);

    int i = lo, j = mid;
    for (int k = lo; k < hi; k++) {
        // <= takes the left element on ties, which keeps the sort stable
        if (j == hi || (i < mid && nums[i] <= nums[j])) {
            buf[k] = nums[i];
            i++;
        } else {
            buf[k] = nums[j];
            j++;
        }
    }

    for (int k = lo; k < hi; k++) { nums[k] = buf[k]; }
}

// Merge sort: halves, then a two-pointer merge. T: O(n log n), S: O(n).
vector<int> sortArray(vector<int> nums) {
    int n = nums.size();
    vector<int> buf(n);
    mergeSort(nums, buf, 0, n);
    return nums;
}

// sort nums[lo, hi]: three-way partition around a random pivot, recurse on < and >
// T: O(n log n) expected, S: O(log n) expected.
void quickSort(vector<int>& nums, int lo, int hi) {
    if (lo >= hi) { return; }
    int pivot = nums[lo + rand() % (hi - lo + 1)];

    // nums[lo, lt) < pivot, nums[lt, i) == pivot, nums(gt, hi] > pivot
    int lt = lo, i = lo, gt = hi;
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

    quickSort(nums, lo, lt - 1);
    quickSort(nums, gt + 1, hi);
}

// Quicksort: the random pivot avoids sorted-input worst cases, the three-way partition
// avoids the all-equal one. T: O(n log n) expected, S: O(log n) expected.
vector<int> sortArrayQuick(vector<int> nums) {
    int n = nums.size();
    quickSort(nums, 0, n - 1);
    return nums;
}

// push nums[i] down within the max-heap nums[0, n) until it beats both children
// T: O(log n), S: O(1).
void siftDown(vector<int>& nums, int i, int n) {
    while (true) {
        int big = i, l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && nums[l] > nums[big]) { big = l; }
        if (r < n && nums[r] > nums[big]) { big = r; }
        if (big == i) { return; }
        swap(nums[i], nums[big]);
        i = big;
    }
}

// Heap sort: build a max-heap in place, then swap the max to the end n - 1 times.
// T: O(n log n), S: O(1).
vector<int> sortArrayHeap(vector<int> nums) {
    int n = nums.size();
    for (int i = n / 2 - 1; i >= 0; i--) { siftDown(nums, i, n); }

    for (int end = n - 1; end > 0; end--) {
        swap(nums[0], nums[end]);
        siftDown(nums, 0, end);
    }

    return nums;
}

void test(const string& label, const vector<int>& nums, const vector<int>& want) {
    bool ok = sortArray(nums) == want && sortArrayQuick(nums) == want
              && sortArrayHeap(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dups-negatives", {3, -1, 3, 0, -1, 2}, {-1, -1, 0, 2, 3, 3});
    test("all-equal", {5, 5, 5, 5}, {5, 5, 5, 5});
    test("reversed", {4, 3, 2, 1}, {1, 2, 3, 4});
    test("empty", {}, {});
}
