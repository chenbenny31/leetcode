// LC 215 Kth Largest Element in an Array (Medium). Pattern: k-min-heap / quickselect.
// Return the k-th largest element of nums (k-th in sorted order, duplicates count).

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
using std::abs;

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

// LC 462: +-1 moves; the median minimizes the sum of |x - t| (each lo / hi pair costs
// hi - lo for any t between them). T: O(n) average, S: O(n) (quickSelect's copy).
long long minMovesToEqual(const vector<int>& nums) {
    int n = nums.size();
    // k-th largest with k = n - n / 2 is ascending index n / 2, the median
    int median = quickSelect(nums, n - n / 2);
    long long moves = 0;
    for (int num : nums) {
        moves += abs((long long)num - median);
    }
    return moves;
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

void test(const string& label, const vector<int>& nums, int k, int want) {
    bool ok = findKthLargest(nums, k) == want && quickSelect(nums, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testMoves(const string& label, const vector<int>& nums, long long want) {
    cout << label << (minMovesToEqual(nums) == want ? " PASS\n" : " FAIL\n");
}

void testSort(const string& label, const vector<int>& nums, const vector<int>& want) {
    cout << label << (sortArray(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("distinct", {7, 1, 5, 3}, 2, 5);
    test("dups", {5, 5, 1, 3}, 3, 3);
    test("all-equal", {7, 7, 7, 7}, 2, 7);
    test("negatives", {-1, -5, -3}, 3, -5);
    testMoves("moves-odd", {1, 5, 2}, 4);
    testMoves("moves-even", {1, 2, 9, 10}, 16);
    testMoves("moves-overflow", {-1000000000, 0, 1000000000}, 2000000000LL);
    testMoves("moves-equal", {3, 3, 3}, 0);
    testSort("sort-reversed", {5, 4, 3, 2, 1}, {1, 2, 3, 4, 5});
    testSort("sort-dups-negatives", {3, -1, 3, 0, -1}, {-1, -1, 0, 3, 3});
    testSort("sort-one", {7}, {7});
}

// follow-up: worst-case O(n) -> median-of-medians pivot (groups of 5); in practice
//   nth_element(nums.begin(), nums.begin() + n - k, nums.end()) (introselect)
// follow-up: top k elements -> after quickselect nums[n-k, n) holds them, O(n)
// follow-up: increment-only moves (LC 3736) -> raise everything to the max:
//   max * n - sum (long long)
// follow-up: quicksort for LC 912 -> reuse the quickselect partition and recurse on both
//   sides; needs a random pivot (rand()) or sorted input makes it O(n^2)
// follow-up: last stone weight (LC 1046) -> max-heap simulation: pop the two heaviest,
//   push their difference if nonzero
