// LC 480 Sliding Window Median: for every window of size k over nums, return its median
// (the middle value, or the mean of the two middle values when k is even).
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cmath>
using std::cout;
using std::string;
using std::vector;
using std::multiset;
using std::fabs;

// Two balanced multisets: lo holds the smaller half (its max is the lower median), hi the
// larger half. Insert and erase by value keep every window update O(log k).
// T: O(n log k), S: O(k).
vector<double> medianSlidingWindow(vector<int>& nums, int k) {
    int n = nums.size();
    multiset<int> lo, hi;   // lo <= hi elementwise; lo.size() == (window size + 1) / 2
    vector<double> result;
    for (int i = 0; i < n; i++) {
        if (lo.empty() || nums[i] <= *lo.rbegin()) {
            lo.insert(nums[i]);
        } else {
            hi.insert(nums[i]);
        }
        if (i >= k) {
            int out = nums[i - k];
            if (out <= *lo.rbegin()) {
                lo.erase(lo.find(out));
            } else {
                hi.erase(hi.find(out));
            }
        }
        int total = lo.size() + hi.size();
        while ((int)lo.size() > (total + 1) / 2) {
            hi.insert(*lo.rbegin());
            lo.erase(lo.find(*lo.rbegin()));
        }
        while ((int)lo.size() < (total + 1) / 2) {
            lo.insert(*hi.begin());
            hi.erase(hi.begin());
        }
        if (i < k - 1) { continue; }
        // halve before adding: the sum of two ints can overflow
        if (k % 2 == 1) {
            result.push_back(*lo.rbegin());
        } else {
            result.push_back(*lo.rbegin() / 2.0 + *hi.begin() / 2.0);
        }
    }
    return result;
}

void test(const string& label, vector<int> nums, int k, const vector<double>& want) {
    vector<double> got = medianSlidingWindow(nums, k);
    bool ok = got.size() == want.size();
    for (int i = 0; ok && i < (int)want.size(); i++) {
        ok = fabs(got[i] - want[i]) < 1e-9;
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd-window", {1, 4, 2, 3}, 3, {2.0, 3.0});
    test("even-window", {1, 2, 3, 4}, 2, {1.5, 2.5, 3.5});
    test("duplicates", {5, 5, 5, 1}, 2, {5.0, 5.0, 3.0});
    test("overflow", {2147483647, 2147483647}, 2, {2147483647.0});
}
