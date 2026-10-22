// two-pointers (read + write), T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    int removeDuplicates(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        int write = 0;

        for (int read = 0; read < n; read++) {
            if (write < 2 || nums[read] != nums[write - 2]) {
                nums[write] = nums[read];
                write++;
            }
        }
        return write;
    }
};
