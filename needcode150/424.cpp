// sliding-window + flat-array, T: O(n), S: O(1) stack-alloc

#include <string>
#include <cstring> // std::memset
#include <algorithm> // std::max

class Solution {
public:
    int characterReplacement(std::string s, int k) {
        constexpr int R = 26;
        constexpr char BASE = 'A';

        int freq[R];
        std::memset(freq, 0, sizeof(freq));

        int left = 0;
        int max_freq = 0;
        int best = 0;

        for (int right = 0; right < static_cast<int>(s.size()); right++) {
            int rc = s[right] - BASE;
            freq[rc]++;
            max_freq = std::max(max_freq, freq[rc]);

            if ((right - left + 1) - max_freq > k) {
                int lc = s[left] - BASE;
                freq[lc]--;
                left++;
            }

            best = std::max(best, right - left + 1);
        }
        return best;
    }
};

// max_freq never decre: win size only grows when higher max_freq found
// if not while for shrinking win: win validity deficit incre by at most one per right adv
// cache freq[26] = 104 bytes, fits in one or two cache lines always L1
// (right - left + 1) - max_freq gives min replacement
// extend to k >= n: win never shrinks
