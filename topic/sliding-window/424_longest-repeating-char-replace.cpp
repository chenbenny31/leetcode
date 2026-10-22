// sliding-window + freq-array, T: O(n), S: O(1)

#include <string>
#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int characterReplacement(std::string s, int k) {
        constexpr int R = 26;
        constexpr char BASE = 'A';
        std::vector<int> freq(R, 0);

        int maxFreq = 0; // high-water mark, neve decre
        int len = 0;

        int left = 0;
        for (int right = 0; right < static_cast<int>(s.length()); right++) {
            int ri = s[right] - BASE;
            freq[ri]++;
            maxFreq = std::max(maxFreq, freq[ri]);

            if ((right - left + 1) - maxFreq > k) { // test with replacement of k
                int li = s[left] - BASE;
                freq[li]--;
                left++;
            }
            len = std::max(len, right - left + 1);
        }
        return len;
    }
};
