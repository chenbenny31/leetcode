// hash-map, T: O(n), S: O(n)

// binary-search per element, T: O(nlogn), S: O(1)

// two-pointers, T: O(n) S: O(1)

#include <vector>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& numbers, int target) {
        int left = 0;
        int right = static_cast<int>(numbers.size()) - 1;

        while (left < right) {
            int sum = numbers[left] + numbers[right];
            if (sum == target) { return {left+1, right+1}; }
            else if (sum < target) { left++; }
            else { right--; }
        }
        return {};
    }
};

// correctness: sortedness gives monotonicity for free
