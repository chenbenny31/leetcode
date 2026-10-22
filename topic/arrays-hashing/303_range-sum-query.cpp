// prefix-sum, T: O(n), S: O(n)

#include <vector>

class NumArray {
private:
    std::vector<int> prefix; // prefix[i] = sum of nums[0..i), prefix[0] = 0

public:
    NumArray(std::vector<int>& nums) : prefix(nums.size() + 1, 0) {
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
    }

    int sumRange(int left, int right) {
        return prefix[right + 1] - prefix[left]; // sum[0..right] - sum[0..left)
    }
};
