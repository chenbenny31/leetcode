// monotonic-stack + flat-array, T: O(m + n), S: O(m)

#include <vector>
#include <cstring> // std::memset

class Solution {
public:
    std::vector<int> nextGreaterElement(std::vector<int>& nums1,
                                        std::vector<int>& nums2) {
        constexpr int R = 10001;
        int nextgt[R]; // flat-array: nextgt[val] = its next greater val
        std::memset(nextgt, -1, sizeof(nextgt));

        int stk[R]; // stk stores val hasn't find next greater 
        int top = -1; // flat-stack stores values

        for (int x : nums2) {
            while (top != -1 && x > stk[top]) {
                nextgt[stk[top]] = x;
                --top;
            }
            stk[++top] = x;
        }

        std::vector<int> out;
        out.reserve(nums1.size());
        for (int x : nums1) { out.push_back(nextgt[x]); }
        return out;
    }
};

// flat-array over hash-map
// stk[R] stores values: need val-key lookup into nextgreater
