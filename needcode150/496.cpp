// monotonic-stack + flat-array, T: O(m + n), S: O(m)

#include <vector>
#include <cstring> // std::memset
#include <stack>

class Solution {
public:
    std::vector<int> nextGreaterElement(std::vector<int>& nums1,
                                        std::vector<int>& nums2) {
        constexpr int N = 10001;
        std::vector<int> next_gt(N, -1);

        std::vector<int> buf;
        buf.reserve(nums2.size());
        std::stack<int, std::vector<int>> stk(std::move(buf)); // mono-decre vals of nums2[]

        for (int x : nums2) {
            while (!stk.empty() && x > stk.top()) {
                next_gt[stk.top()] = x;
                stk.pop();
            }
            stk.push(x);
        }

        std::vector<int> out;
        out.reserve(nums1.size());
        for (int x : nums1) { out.push_back(next_gt[x]); }
        return out;
    }
};

// store vals not indices: next_gt is a map from val to val of nums2[]
// next_gt init to -1 to represent a hit miss of val
// vector not raw type[] of next_gt: same cache behavior (L1/L2), no stack pressure, min func call overhead
