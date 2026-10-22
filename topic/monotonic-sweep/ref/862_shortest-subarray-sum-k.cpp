// LC 862 Shortest Subarray with Sum at Least K: nums may contain negatives. Return the
// length of the shortest non-empty subarray with sum >= k, or -1 if none exists.
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>
#include <climits>
using std::cout;
using std::deque;
using std::min;
using std::string;
using std::vector;

// prefix sums + monotonic deque: the front is the best (latest) start that already
// works for the current end, the back keeps prefix values increasing.
// T: O(n), S: O(n).
int shortestSubarray(const vector<int>& nums, int k) {
    int n = nums.size();
    vector<long long> pre(n + 1, 0);   // pre[i] = sum of nums[0..i-1]
    for (int i = 0; i < n; i++) { pre[i + 1] = pre[i] + nums[i]; }
    deque<int> starts;   // indices into pre with increasing pre values
    int best = INT_MAX;
    for (int i = 0; i <= n; i++) {
        // front works for i; any later end is longer with it, so it is done after this
        while (!starts.empty() && pre[i] - pre[starts.front()] >= k) {
            best = min(best, i - starts.front());
            starts.pop_front();
        }
        // a start with pre >= pre[i] is older and never better than i
        while (!starts.empty() && pre[starts.back()] >= pre[i]) { starts.pop_back(); }
        starts.push_back(i);
    }
    if (best == INT_MAX) { return -1; }
    return best;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    if (shortestSubarray(nums, k) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("negative-inside", {2, -1, 2}, 3, 3);
    test("negative-reset", {84, -37, 32, 40, 95}, 167, 3);
    test("single", {1, 5, 2}, 5, 1);
    test("impossible", {1, -1, 1}, 3, -1);
}
