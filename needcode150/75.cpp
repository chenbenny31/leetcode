// counting-sort, T: O(n), S: O(1)

#include <vector>
#include <cstring> // std::memset

class Solution {
public:
    void sortColors(std::vector<int>& nums) {
        constexpr int R = 3;
        int count[R];
        std::memset(count, 0, sizeof(count));

        for (int x : nums) { count[x]++; }
        
        int idx = 0;
        for (int r = 0; r < R; r++) {
            while (count[r]--) { nums[idx++] = r; }
        }
    }
};

// Dutch National Flag, three-pointers, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::swap

class Solution {
public:
    void sortColors(std::vector<int>& nums) {
        int low = 0;
        int mid = 0;
        int high = static_cast<int>(nums.size()) - 1;

        while (mid <= high) {
            if (nums[mid] == 0) { std::swap(nums[low++], nums[mid++]); } 
            else if (nums[mid] == 1) { mid++; }
            else { std::swap(nums[mid], nums[high--]); }
        }
    }
};

// Dutch National Flag - Dijkstra's algo: same as 3-way partition in quick-select, [0, low), [low, mid), [mid, high], (high, n-1]
// counting-sort prefer: more than 3 colors
