// LC 220 Contains Duplicate III: two values within valueDiff at most indexDiff apart?
#include <iostream>
#include <vector>
#include <string>
#include <set>

using std::cout;
using std::string;
using std::vector;
using std::set;

// Keep the last indexDiff values in an ordered set; the closest one at or above
// x - valueDiff is the only candidate. (Buckets of width valueDiff + 1 give O(n).)
// T: O(n log k) for k = indexDiff, S: O(k).
bool containsNearbyAlmostDuplicate(const vector<int>& nums, int indexDiff,
                                   int valueDiff) {
    int n = nums.size();
    // values of the last indexDiff elements; a repeat would already have returned true
    set<long long> window;
    for (int i = 0; i < n; i++) {
        long long x = nums[i];
        auto it = window.lower_bound(x - valueDiff);
        if (it != window.end() && *it <= x + valueDiff) { return true; }

        window.insert(x);
        if (i >= indexDiff) { window.erase(nums[i - indexDiff]); }
    }

    return false;
}

void test(const string& label, const vector<int>& nums, int indexDiff, int valueDiff,
          bool want) {
    bool got = containsNearbyAlmostDuplicate(nums, indexDiff, valueDiff);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("near-in-window", {1, 5, 9, 4}, 2, 1, true);
    test("index-too-far", {1, 5, 9, 1}, 2, 0, false);
    test("negatives", {-3, 3, -2}, 2, 1, true);
    test("no-overflow", {-1000000000, 1000000000}, 1, 1000000000, false);
}
