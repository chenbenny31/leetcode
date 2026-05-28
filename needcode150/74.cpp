// binary-search on flatten index, T: O(log(mn)), S: O(1)

#include <vector>

class Solution {
public:
    bool searchMatrix(std::vector<std::vector<int>>& matrix, int target) {
        int m = static_cast<int>(matrix.size());
        int n = static_cast<int>(matrix[0].size());
        int lo = 0;
        int hi = m * n - 1;

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            int val = matrix[mid / n][mid % n];

            if (val == target) { return true; }
            else if (val < target) { lo = mid + 1; }
            else { hi = mid - 1; }
        }
        return false;
    }
};

// mid / n and mid % n for flatten matrix
// flat binary search not two sep on row and col
// overflow on m * n
// cache matrix[mid/n][mid%n] intro diff heap alloc and random access, flat to 1-d for perf-critical
