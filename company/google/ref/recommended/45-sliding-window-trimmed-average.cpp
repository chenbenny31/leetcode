// Sliding Window Trimmed Average (Google, interview report)
// integers nums, window size w; one average per full window, left to right
// (a) drop the k largest, average the other w - k (0 <= k < w)
// (b) drop trimCount smallest and trimCount largest, average the rest (2 * trimCount < w)
//
// clarify:
// n, w up to 1e5 -> need O(n log w); brute force is O(n w log w)
// dups -> count separately
// w > n -> empty result
// doubles or rounded ints? -> doubles
// values up to 2^31 - 1 -> sums in long long

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

// (a) drop k largest, brute force
// core idea: copy + sort each window, sum its w - k smallest
// T: O(n w log w), S: O(w)
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

// (a) drop k largest, fast
// core idea: multiset top = k largest, rest = the others w/ a running sum
// new value enters top, top's min drops to rest if over k; on removal refill top
// T: O(n log w), S: O(w)
vector<double> dropLargestAverageFast(const vector<int>& nums, int w, int k) {
    // k largest values of the window
    multiset<int> top;
    // other values of the window; every one <= every value in top
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
            // erase one copy by iterator: erase(value) would remove every copy
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

// (b) trim both ends, brute force
// core idea: copy + sort each window, sum the middle part
// T: O(n w log w), S: O(w)
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

// (b) trim both ends, fast (LC 1825 MKAverage)
// core idea: three multisets split the window by value: low, mid (w/ midSum), high
// add to the set the value belongs to, remove from the set holding it, then fix sizes
// T: O(n log w), S: O(w)
vector<double> trimmedAverageFast(const vector<int>& nums, int w, int trimCount) {
    // trimCount smallest values of the window
    multiset<int> low;
    // middle values; every one >= low's max and <= high's min
    multiset<int> mid;
    // trimCount largest values of the window
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

    testTrim("trim-one", {5, 1, 9, 3}, 3, 1, {5, 3});
    testTrim("trim-two", {7, 1, 4, 9, 2}, 5, 2, {4});
    testTrim("trim-boundary-dup", {2, 9, 2, 5, 2}, 3, 1, {2, 5, 2});
    testTrim("trim-big", {2000000000, 2000000000, 2000000000, 0, 5}, 4, 1,
             {2000000000, 1000000002.5});
}

// follow-up:
// drop by percentage (top 10%) -> k = w / 10 (ask rounding); same code
// median of every window (LC 480) -> (a) w/ k = w / 2: rest's max, or mean w/ top's min
// stream keeping last w values -> fast versions already work; outgoing value from a queue
// value equal to a boundary in (b) -> remove from whichever set find hits; fix midSum
