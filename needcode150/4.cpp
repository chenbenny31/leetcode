// merge-traversal, T: O(m + n), S: O(1)

#include <vector>

class Solution {
public:
    double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
        int m = static_cast<int>(nums1.size());
        int n = static_cast<int>(nums2.size());
        int total = m + n;
        int mid = total / 2;

        int i = 0, j = 0;
        int prev = 0, curr = 0;

        for (int k = 0; k <= mid; k++) {
            prev = curr;
            if (i < m && (j >= n || nums1[i] <= nums2[j])) {
                curr = nums1[i];
                i++;
            } else {
                curr = nums2[j];
                j++;
            }
        }
        return (total % 2 == 1) ? curr : (prev + curr) / 2.0;
    }
};

// binary-search on partition, T: O(log(min(m,n)), S: O(1)

#include <vector>
#include <climits> // INT_MIN, INT_MAX
#include <algorithm> // std::max, std::min

class Solution {
public:
    double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
        if (nums1.size() > nums2.size()) { return findMedianSortedArrays(nums2, nums1); }

        int m = static_cast<int>(nums1.size());
        int n = static_cast<int>(nums2.size());
        int lo = 0, hi = m;

        while (lo <= hi) {
            int cut1 = lo + (hi - lo) / 2;
            int cut2 = (m + n + 1) / 2 - cut1;

            int l1 = (cut1 == 0) ? INT_MIN : nums1[cut1 - 1];
            int r1 = (cut1 == m) ? INT_MAX : nums1[cut1];
            int l2 = (cut2 == 0) ? INT_MIN : nums2[cut2 - 1];
            int r2 = (cut2 == n) ? INT_MAX : nums2[cut2];

            if (l1 <= r2 && l2 <= r1) {
                if ((m + n) % 2 == 1) { return std::max(l1, l2); }
                else { return (static_cast<long long>(std::max(l1, l2)) + std::min(r1, r2)) / 2.0; }
            } else if (l1 > r2) {
                hi = cut1 - 1;
            } else {
                lo = cut1 + 1;
            }
        }
        return 0.0;
    }
};

// core algo: nums1[0..cut1) + nums2[0..cut2) is the first half, need l1 <= r2 && l2 <= l1, case l1 > r2 -> hi = cut1 - 1, case l2 > r1 ->  lo = cut1 + 1
// INT_MIN, INT_MAX sentinels for boundary test
// (m + n + 1) / 2 for odd lentgth
// long long casting for even median incase INT_MIN, INT_MAX involces
// merge vs binary-search: worse time complexity but better cache behavior
