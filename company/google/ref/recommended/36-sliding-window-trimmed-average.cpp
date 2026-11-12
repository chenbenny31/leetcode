// Sliding Window Trimmed Average (Google, interview report): integers nums, window
// size w.
// (a) For every full window (left to right), drop its k largest values and return the
//     average of the other w - k (0 <= k < w).
// (b) Drop the trimCount smallest and the trimCount largest, average the rest
//     (2 * trimCount < w).
// Duplicates count separately; w > nums.size() gives an empty result.

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>

using std::cout;
using std::string;
using std::vector;
using std::multiset;
using std::sort;
using std::abs;

// (a) copy and sort each window, sum its w - k smallest. T: O(n w log w), S: O(w).
vector<double> dropLargestAverage(const vector<int>& nums, int w, int k) {
    vector<double> result;
    for (int start = 0; start + w <= (int)nums.size(); start++) {
        vector<int> window(nums.begin() + start, nums.begin() + start + w);
        sort(window.begin(), window.end());
        long long sum = 0;
        for (int i = 0; i < w - k; i++) { sum += window[i]; }
        result.push_back((double)sum / (w - k));
    }
    return result;
}

// (a) improved: `top` keeps the k largest of the window, `rest` the others with a running
// sum. A new value enters top and top's smallest drops to rest if top is over k; an
// outgoing value leaves its set and top is refilled from rest's largest.
// T: O(n log w), S: O(w).
vector<double> dropLargestAverageFast(const vector<int>& nums, int w, int k) {
    // the k largest values of the window
    multiset<int> top;
    // the other values of the window; every one <= every value in top
    multiset<int> rest;
    // sum of rest
    long long restSum = 0;
    vector<double> result;
    for (int i = 0; i < (int)nums.size(); i++) {
        top.insert(nums[i]);
        if ((int)top.size() > k) {
            int low = *top.begin();
            top.erase(top.begin());
            rest.insert(low);
            restSum += low;
        }
        if (i >= w) {
            int out = nums[i - w];
            auto it = top.find(out);
            if (it != top.end()) {
                top.erase(it);
                int high = *rest.rbegin();
                rest.erase(rest.find(high));
                restSum -= high;
                top.insert(high);
            } else {
                rest.erase(rest.find(out));
                restSum -= out;
            }
        }
        if (i >= w - 1) { result.push_back((double)restSum / (w - k)); }
    }
    return result;
}

// (b) copy and sort each window, sum the middle part. T: O(n w log w), S: O(w).
vector<double> trimmedAverage(const vector<int>& nums, int w, int trimCount) {
    vector<double> result;
    for (int start = 0; start + w <= (int)nums.size(); start++) {
        vector<int> window(nums.begin() + start, nums.begin() + start + w);
        sort(window.begin(), window.end());
        long long sum = 0;
        for (int i = trimCount; i < w - trimCount; i++) { sum += window[i]; }
        result.push_back((double)sum / (w - 2 * trimCount));
    }
    return result;
}

bool near(const vector<double>& got, const vector<double>& want) {
    if (got.size() != want.size()) { return false; }
    for (int i = 0; i < (int)got.size(); i++) {
        if (abs(got[i] - want[i]) > 1e-9) { return false; }
    }
    return true;
}

void test(const string& label, const vector<int>& nums, int w, int k,
          const vector<double>& want) {
    bool ok = near(dropLargestAverage(nums, w, k), want) &&
              near(dropLargestAverageFast(nums, w, k), want);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testTrim(const string& label, const vector<int>& nums, int w, int trimCount,
              const vector<double>& want) {
    bool ok = near(trimmedAverage(nums, w, trimCount), want);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("top-leaves", {9, 1, 2, 3}, 2, 1, {1, 1, 2});
    test("keep-all", {1, 2, 3}, 2, 0, {1.5, 2.5});
    test("duplicates", {5, 5, 1, 5}, 3, 1, {3, 3});
    test("big-values", {2000000000, 2000000000, 1}, 3, 1, {1000000000.5});
    test("short", {1, 2}, 3, 1, {});

    testTrim("trim-one", {5, 1, 9, 3}, 3, 1, {5, 3});
    testTrim("trim-zero", {1, 2, 3}, 3, 0, {2});
    testTrim("trim-two", {7, 1, 4, 9, 2}, 5, 2, {4});
}

// follow-up: (b) in O(n log w) -> three multisets low / mid / high (LC 1825 MKAverage):
//   low and high hold trimCount values each, mid the rest with midSum. Add x into mid,
//   then move mid's min to low and mid's max to high while they are short, swapping
//   across a boundary when out of order; an outgoing value leaves its set and the short
//   side is refilled from mid. Average = midSum / (w - 2 * trimCount).
// follow-up: drop by percentage (top 10%) -> k = w / 10, same structures
// follow-up: streaming with only the last w values -> the fast version already is one;
//   keep a deque of the last w values instead of indexing nums[i - w]
