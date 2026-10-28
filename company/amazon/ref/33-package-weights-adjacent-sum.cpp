// Each package weight is a digit 0..9. Can the packages be ordered so that every two
// adjacent weights sum to less than 10?
// Follow-up: solve it in O(n) using the fixed weight range.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Two weights >= 5 can never touch, so the "big" ones must sit in gaps of the small ones
// (<= 4), at most one per gap. With smalls in ascending order the gaps ask for the least:
// the left end needs a big b with 9 - b >= s1, the gap between s(i-1), s(i) needs
// 9 - b >= s(i), the right end needs 9 - b >= s(last). Then match the most restrictive
// bigs to the easiest gaps: sorted capacities must cover sorted needs pairwise.
bool feasible(vector<int> need, vector<int> cap) {
    if (cap.size() > need.size()) { return false; }
    sort(need.begin(), need.end());
    sort(cap.begin(), cap.end());
    int b = cap.size();
    for (int i = 0; i < b; i++) {
        if (cap[i] < need[i]) { return false; }
    }
    return true;
}

// straightforward: sort the small weights. T: O(n log n), S: O(n).
bool canArrange(const vector<int>& weights) {
    vector<int> small, cap;
    for (int w : weights) {
        if (w <= 4) {
            small.push_back(w);
        } else {
            cap.push_back(9 - w);
        }
    }
    if (small.empty()) { return cap.size() <= 1; }
    sort(small.begin(), small.end());
    // need = threshold of each gap: s1 (left end), s2..s(last) (between neighbors),
    // s(last) (right end)
    vector<int> need = small;
    need.push_back(small.back());
    return feasible(need, cap);
}

// improved: counting sort over the 10 digits builds the same sorted lists.
// T: O(n), S: O(n).
bool canArrangeCount(const vector<int>& weights) {
    // cnt[d] = packages of weight d
    int cnt[10] = {};
    for (int w : weights) { cnt[w]++; }
    vector<int> small, cap;
    for (int d = 0; d <= 4; d++) { small.insert(small.end(), cnt[d], d); }
    for (int d = 9; d >= 5; d--) { cap.insert(cap.end(), cnt[d], 9 - d); }
    if (small.empty()) { return cap.size() <= 1; }
    vector<int> need = small;
    need.push_back(small.back());
    return feasible(need, cap);
}

void test(const string& label, const vector<int>& weights, bool want) {
    bool ok = canArrange(weights) == want && canArrangeCount(weights) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("all-small", {4, 4, 3}, true);
    test("two-big", {5, 5}, false);
    test("one-big", {9}, true);
    test("separated", {9, 0, 5, 0}, true);
    test("tight", {9, 1, 8}, false);
    test("gap-fit", {8, 1, 0, 7}, true);
}
