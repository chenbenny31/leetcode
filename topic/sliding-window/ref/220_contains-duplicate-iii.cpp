// LC 220 Contains Duplicate III: two values within valueDiff at most indexDiff apart?
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::set;
using std::unordered_map;

// Keep the last indexDiff values in an ordered set; the closest one at or above
// x - valueDiff is the only candidate. T: O(n log k) for k = indexDiff, S: O(k).
bool containsNearbyAlmostDuplicate(const vector<int>& nums, int indexDiff,
                                   int valueDiff) {
    // values of the last indexDiff elements; a repeat would already have returned true
    set<long long> window;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        long long x = nums[i];
        auto it = window.lower_bound(x - valueDiff);
        if (it != window.end() && *it <= x + valueDiff) { return true; }

        window.insert(x);
        if (i >= indexDiff) { window.erase(nums[i - indexDiff]); }
    }

    return false;
}

// bucket of x among [id * w, id * w + w), rounding down for negatives
long long bucketOf(long long x, long long w) {
    if (x >= 0) { return x / w; }
    return (x + 1) / w - 1;
}

// Buckets of width valueDiff + 1: two values in one bucket always match, so only the
// same and the two neighbouring buckets need checking. T: O(n), S: O(k).
bool containsNearbyAlmostDuplicateBuckets(const vector<int>& nums, int indexDiff,
                                          int valueDiff) {
    long long w = 1LL * valueDiff + 1;
    // bucket id -> the one window value in it
    unordered_map<long long, long long> buckets;
    int n = nums.size();

    for (int i = 0; i < n; i++) {
        long long x = nums[i];
        long long id = bucketOf(x, w);
        if (buckets.count(id)) { return true; }

        auto left = buckets.find(id - 1);
        if (left != buckets.end() && x - left->second <= valueDiff) { return true; }
        auto right = buckets.find(id + 1);
        if (right != buckets.end() && right->second - x <= valueDiff) { return true; }

        buckets[id] = x;
        if (i >= indexDiff) { buckets.erase(bucketOf(nums[i - indexDiff], w)); }
    }

    return false;
}

void test(const string& label, const vector<int>& nums, int indexDiff, int valueDiff,
          bool want) {
    bool ok = containsNearbyAlmostDuplicate(nums, indexDiff, valueDiff) == want
              && containsNearbyAlmostDuplicateBuckets(nums, indexDiff, valueDiff) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("near-in-window", {1, 5, 9, 4}, 2, 1, true);
    test("index-too-far", {1, 5, 9, 1}, 2, 0, false);
    test("negatives", {-3, 3, -2}, 2, 1, true);
    test("no-overflow", {-1000000000, 1000000000}, 1, 1000000000, false);
}
