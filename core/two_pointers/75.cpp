// counting-sort, T: O(n) two pass, S: O(1)

#include <vector>

class Solution {
public:
    void sortColors(std::vector<int>& nums) {
        int count0 = 0;
        int count1 = 0;
        int count2 = 0;

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            if (nums[i] == 0) { count0++; }
            else if (nums[i] == 1) { count1++; }
            else { count2++; }
        }

        int idx = 0;
        for (int i = 0; i < count0; i++) { nums[idx] = 0; idx++; }
        for (int i = 0; i < count1; i++) { nums[idx] = 1; idx++; }
        for (int i = 0; i < count2; i++) { nums[idx] = 2; idx++; }
    }
};

// dutch-national-flag, T: O(n) one pass, S: O(1)

#include <vector>
#include <utility> // std::swap

class Solution {
public:
    void sortColors(std::vector<int>& nums) {
        int lo = 0; // [0..lo) for 0
        int i = lo; // [lo..hi] for 1
        int hi = static_cast<int>(nums.size()) - 1; // [hi..n) for 2

        while (i <= hi) {
            if (nums[i] == 0) { std::swap(nums[lo++], nums[i++]); }
            else if (nums[i] == 2) { std::swap(nums[hi--], nums[i]); }
            else { i++; }
        }
    }
};

// sort a fixed small alphabet lead to quick-sort(3way) / counting-sort that beats O(nlogn) sorting
