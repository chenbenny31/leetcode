// Sliding Window Trimmed Average (Google, interview report): integers nums, window
// size w.
// (a) For every full window (left to right), drop its k largest values and return the
//     average of the other w - k (0 <= k < w).
// (b) Drop the trimCount smallest and the trimCount largest, average the rest
//     (2 * trimCount < w).
// Duplicates count separately; w > nums.size() gives an empty result.
//
// Clarify:
// - n and w up to ~1e5 -> sorting every window is O(n w log w), too slow when w ~ n;
//   state it, then keep the window split in sorted sets: O(n log w)
// - duplicates -> every copy counts (multiset, never set)
// - k = 0 / trimCount = 0 -> plain average; k < w and 2 * trimCount < w, so the divisor
//   is never 0
// - w > n -> no full window, empty result; w == n -> one value
// - return doubles? -> yes; if an integer is wanted, ask floor or round
// - values up to 2^31 - 1 -> window sums need long long
// Idea (a): two multisets split the window by value: top = its k largest, rest = the
//   others with restSum. Each step moves one value in and one out with O(1) moves across
//   the boundary, so the average is restSum / (w - k) with no re-summing.
// Idea (b): the same with three sets low / mid / high and midSum (trimmedAverageFast).
// Pitfalls:
// - multiset::erase(value) removes every copy; erase(find(value)) removes one
// - (a) a new value goes into top first, then top's smallest drops to rest; an outgoing
//   value found in top is replaced by rest's largest
// - (a) rest is never empty at that refill: the window holds w + 1 values at that moment
// - record only after the outgoing value has left (i >= w - 1)
// - (b) a value equal to a boundary can sit in either neighboring set: remove it from
//   whichever set find hits, and change midSum only for mid
// - (b) shrink low / high to trimCount first, then refill them from mid
// - sum in long long; cast to double before dividing
// Tests to write: (a) the window max leaves; k = 0; duplicates of the dropped value;
//   sum past INT_MAX; w > n. (b) trim one / zero / two; a duplicate on the trim
//   boundary; big values; w > n.

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

// (b) improved (LC 1825 MKAverage): three multisets split the window by value, low (the
// trimCount smallest), mid (with midSum) and high (the trimCount largest). A new value
// goes into the set its value belongs to, an outgoing one leaves the set holding it, then
// sizes are restored by moving values across the low/mid and mid/high boundaries.
// T: O(n log w), S: O(w).
vector<double> trimmedAverageFast(const vector<int>& nums, int w, int trimCount) {
    // the trimCount smallest values of the window
    multiset<int> low;
    // the middle values; every one is >= low's max and <= high's min
    multiset<int> mid;
    // the trimCount largest values of the window
    multiset<int> high;
    // sum of mid
    long long midSum = 0;
    vector<double> result;
    for (int i = 0; i < (int)nums.size(); i++) {
        int x = nums[i];
        if (!low.empty() && x < *low.rbegin()) {
            low.insert(x);
        } else if (!high.empty() && x > *high.begin()) {
            high.insert(x);
        } else {
            mid.insert(x);
            midSum += x;
        }
        if (i >= w) {
            int out = nums[i - w];
            auto lowIt = low.find(out);
            auto highIt = high.find(out);
            if (lowIt != low.end()) {
                low.erase(lowIt);
            } else if (highIt != high.end()) {
                high.erase(highIt);
            } else {
                mid.erase(mid.find(out));
                midSum -= out;
            }
        }
        // shrink low and high to trimCount, then refill them from mid
        while ((int)low.size() > trimCount) {
            int v = *low.rbegin();
            low.erase(low.find(v));
            mid.insert(v);
            midSum += v;
        }
        while ((int)high.size() > trimCount) {
            int v = *high.begin();
            high.erase(high.begin());
            mid.insert(v);
            midSum += v;
        }
        while ((int)low.size() < trimCount && !mid.empty()) {
            int v = *mid.begin();
            mid.erase(mid.begin());
            midSum -= v;
            low.insert(v);
        }
        while ((int)high.size() < trimCount && !mid.empty()) {
            int v = *mid.rbegin();
            mid.erase(mid.find(v));
            midSum -= v;
            high.insert(v);
        }
        if (i >= w - 1) { result.push_back((double)midSum / (w - 2 * trimCount)); }
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
    bool ok = near(trimmedAverage(nums, w, trimCount), want) &&
              near(trimmedAverageFast(nums, w, trimCount), want);
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
    testTrim("trim-boundary-dup", {2, 9, 2, 5, 2}, 3, 1, {2, 5, 2});
    testTrim("trim-big", {2000000000, 2000000000, 2000000000, 0, 5}, 4, 1,
             {2000000000, 1000000002.5});
    testTrim("trim-short", {1, 2}, 3, 1, {});
}

// follow-up: (b) in O(n log w) (LC 1825 MKAverage) -> trimmedAverageFast above
// follow-up: drop by percentage (top 10%) -> k = w / 10 (ask: round down?), same code;
//   both ends by a percentage -> trimCount = w * pct / 100
// follow-up: median of every window (LC 480) -> dropLargestAverageFast's sets with
//   k = w / 2: rest holds the smaller half, its max is the middle value
//     double median = w % 2 == 1 ? *rest.rbegin()
//                                : (*rest.rbegin() + (double)*top.begin()) / 2;
// follow-up: a stream with only the last w values kept -> the fast versions already are
//   one; read the outgoing value from a queue of the last w values, not nums[i - w]:
//     last.push(x);
//     if ((int)last.size() > w) { int out = last.front(); last.pop(); /* remove out */ }
