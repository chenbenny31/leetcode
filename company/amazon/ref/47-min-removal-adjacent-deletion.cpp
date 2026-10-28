// Repeat until nums is empty: take the smallest remaining value (smallest index on ties),
// add it to the score, and remove it together with its nearest remaining neighbor on
// each side (the array is linear, not circular). Return the total score.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::greater;

// straightforward: keep the remaining indices in a vector and scan for the minimum.
// T: O(n^2), S: O(n).
long long totalScore(const vector<int>& nums) {
    // indices still present, in order
    vector<int> alive;
    int n = nums.size();
    for (int i = 0; i < n; i++) { alive.push_back(i); }
    long long score = 0;
    while (!alive.empty()) {
        int p = 0, a = alive.size();
        for (int q = 1; q < a; q++) {
            if (nums[alive[q]] < nums[alive[p]]) { p = q; }
        }
        score += nums[alive[p]];
        int from = p > 0 ? p - 1 : p;
        int to = p + 1 < a ? p + 1 : p;
        alive.erase(alive.begin() + from, alive.begin() + to + 1);
    }
    return score;
}

// improved: min-heap of (value, index) with lazy deletion, and prev/next arrays as a
// doubly linked list of the survivors. T: O(n log n), S: O(n).
long long totalScoreHeap(const vector<int>& nums) {
    int n = nums.size();
    // prv[i] / nxt[i] = nearest remaining index to the left / right, -1 or n if none
    vector<int> prv(n), nxt(n);
    for (int i = 0; i < n; i++) {
        prv[i] = i - 1;
        nxt[i] = i + 1;
    }
    vector<bool> removed(n, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    for (int i = 0; i < n; i++) { heap.push({nums[i], i}); }
    long long score = 0;
    while (!heap.empty()) {
        int i = heap.top().second;
        heap.pop();
        if (removed[i]) { continue; }
        score += nums[i];
        int left = prv[i], right = nxt[i];
        removed[i] = true;
        if (left >= 0) {
            removed[left] = true;
            left = prv[left];
        }
        if (right < n) {
            removed[right] = true;
            right = nxt[right];
        }
        // stitch the outer survivors together
        if (left >= 0) { nxt[left] = right; }
        if (right < n) { prv[right] = left; }
    }
    return score;
}

void test(const string& label, const vector<int>& nums, long long want) {
    bool ok = totalScore(nums) == want && totalScoreHeap(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-rounds", {3, 1, 2, 4}, 5);
    test("rejoin", {2, 3, 1, 5, 4}, 3);
    test("tie-index", {1, 1, 1}, 2);
    test("edge-min", {4, 3, 2, 1}, 4);
    test("single", {5}, 5);
}
