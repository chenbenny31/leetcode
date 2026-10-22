// binary-search per element, T: O(nlogn), S: O(1)

#include <vector>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& numbers, int target) {
        const int n = static_cast<int>(numbers.size());

        for (int i = 0; i < n; i++) {
            int comp = target - numbers[i];
            int lo = i + 1;
            int hi = n - 1;

            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                if (numbers[mid] == comp) { return {i + 1, mid + 1}; }
                else if (numbers[mid] < comp) { lo = mid + 1; }
                else { hi = mid - 1; }
            }
        }
        return {-1, -1};
    }
};

// two-pointers, T: O(n) S: O(1)

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
        return {-1, -1};
    }
};
