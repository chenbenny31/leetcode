// sliding-window + freq-array, T: O(n), S: O(1)

#include <string>
#include <algorithm> // std::max

class Solution {
public:
    int characterReplacement(std::string s, int k) {
        int freq[26] = {};
        int left = 0;
        int maxFreq = 0;
        int bestLen = 0;

        for (int right = 0; right < static_cast<int>(s.length()); right++) {
            freq[s[right] - 'A']++;
            maxFreq = std::max(maxFreq, freq[s[right] - 'A']);

            if ((right - left + 1) - maxFreq > k) {
                freq[s[left] - 'A']--;
                left++;
            }
            bestLen = std::max(bestLen, right - left + 1);
        }
        return bestLen;
    }
};

// maxFreq: is the high-water mark, keep it stale after a shrink, a larger maxFreq make winSize-maxFreq smaller
