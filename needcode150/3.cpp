// sliding-window + flat-array, T: O(n), S: O(1)

#include <string>
#include <cstring> // std::memset
#include <algorithm> // std::max

class Solution {
public:
    int lengthOfLongestSubstring(std::string s) {
        constexpr int R = 128;
        int last[R];
        std::memset(last, -1, sizeof(last));

        int left = 0;
        int best = 0;
        for (int right = 0; right < static_cast<int>(s.size()); right++) {
            int c = static_cast<unsigned char>(s[right]);
            if (last[c] >= left) { left = last[c] + 1; }
            last[c] = right;
            best = std::max(best, right - left + 1);
        }
        return best;
    }
};

// direct jump left = last[c] + 1: jumps directly to a valid left boundary in O(1)
// memset(last, -1): -1 fills byte with 0xFF
// unsigned char casting: signed char val > 127 produce neg indices
// cache last[128] = 512 bytes, fits in L1
// extend to Unicode: need hash-map due to 1M+ code points
