// backward two-pointer merge, T: O(m+n), S: O(1)

#include <vector>

class Solution {
public:
    void merge(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n) {
        int read1 = m - 1;
        int read2 = n - 1;
        int write = m + n - 1;

        while (read2 >= 0) { // only need to drain nums2
            if (read1 >= 0 && nums1[read1] > nums2[read2]) {
                nums1[write] = nums1[read1];
                read1--;
            } else {
                nums1[write] = nums2[read2];
                read2--;
            }
            write--;
        }
    }
};
