// binary-search on value range, T: O(nlogn), S: O(1)

#include <vector>

class Solution {
public:
    int findDuplicate(std::vector<int>& nums) {
        int lo = 1;
        int hi = static_cast<int>(nums.size()) - 1;

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            int count = 0;

            for (int x : nums) {
                if (x <= mid) { ++count; }
            }

            if (count > mid) { hi = mid; }
            else { lo = mid + 1; }
        }
        return lo;
    }
};

// bit-counting, T: O(nlogn), S: O(1)

#include <vector>

class Solution {
public:
    int findDuplicate(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size()) - 1;
        int dup = 0;
        constexpr int BITS = 32;

        for (int bit = 0; bit < BITS; bit++) {
            int mask = 1 << bit;
            int expect = 0; // count of nums in [1,n] with this bit set
            int actual = 0; // count in nums

            for (int i = 1; i <= n; i++) {
                if (i & mask) { ++expect; }
            }
            for (int x : nums) {
                if (x & mask) { ++actual; }
            }

            if (actual > expect) { dup |= mask; }
        }
        return dup;
    }
};

// Floyd's cycle detection, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    int findDuplicate(std::vector<int>& nums) {
        // find intersection inside cycle
        int slow = nums[0];
        int fast = nums[0];
        do {
            slow = nums[slow];
            fast = nums[nums[fast]];
        } while (slow != fast);

        // find cycle entry - the dup val
        slow = nums[0];
        while (slow != fast) {
            slow = nums[slow];
            fast = nums[fast];
        }
        return slow;
    }
};

// binary-search pigeonhole: count <= mid exceeds mid -> dup exist in [1, mid]
// Floyd's correctness: vals in [1, n], arr len of n+1, i -> nums[i] forms a jumping circle
