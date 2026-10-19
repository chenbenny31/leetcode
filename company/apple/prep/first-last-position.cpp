// return [first, last] idx of target or [-1, -1] if none, nums is sorted ascending

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// first index with nums[i] >= target, T: O(logn), S: O(1)
int lowerBound(const vector<int>& nums, int target) {
    int lo = 0 ;
    int hi = nums.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

// first index with nums[i] > target, T: O(logn), S: O(1)
int upperBound(const vector<int>& nums, int target) {
    int lo = 0 ;
    int hi = nums.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] <= target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

// T: O(logn), S: O(1)
vector<int> searchRange(const vector<int>& nums, int target) {
    const int n = nums.size();
    int firstEq = lowerBound(nums, target);
    if (firstEq == n || nums[firstEq] != target) { return {-1, -1}; } // prune
    int firstGt = upperBound(nums, target);
    return {firstEq, firstGt - 1};
}

void test(const string& label, const vector<int>& nums, int target,
          const vector<int>& want) {
    cout << label << (searchRange(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {1, 2, 2, 3}, 2, {1, 2});
    test("same", {2, 2}, 2, {0, 1});
    test("missing", {1, 3}, 2, {-1, -1});
    test("past-end", {1, 3}, 5, {-1, -1});
}
