// two binary-search, T: O(log(n)), S: O(1)

#include <vector>
#include <algorithm> // std::lower_bound, std::upper_bound

class Solution {
public:
    std::vector<int> searchRange(std::vector<int>& nums, int target) {
        auto first = std::lower_bound(nums.begin(), nums.end(), target);
        if (first == nums.end() || *first != target) { return {-1, -1}; }

        auto last = std::upper_bound(nums.begin(), nums.end(), target);
        return {static_cast<int>(first - nums.begin()),
                static_cast<int>(last - nums.begin()) - 1};
    }
};

// self-impl binary-search, T: O(logn), S: O(1)

#include <vector>

class Solution {
public:
    std::vector<int> searchRange(std::vector<int>& nums, int target) {
        int n = static_cast<int>(nums.size());
        int first = lower_bound(nums, 0, n, target);
        if (first == n || nums[first] != target) { return {-1, -1}; }
        int last = upper_bound(nums, first, n, target) - 1;
        return {first, last};
    }

private:
    int lower_bound(std::vector<int>& nums, int lo, int hi, int target) {
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] < target) { lo = mid + 1; }
            else { hi = mid; }
        }
        return lo;
    }

    int upper_bound(std::vector<int>& nums, int lo, int hi, int target) {
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] <= target) { lo = mid + 1; }
            else { hi = mid; }
        }
        return lo;
    }
};

// lower_bound vs upper_bound: first elem >= target vs first elem > target
// upper_bound starts from first: halves search range for second call
// cache two binary search on same array: first warms cache lines near target, second benefits
// STL vs self-impl: STL is idiomatic, well-tested and compile to the same code
