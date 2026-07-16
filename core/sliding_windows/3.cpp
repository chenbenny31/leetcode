// sliding-window + hash-set, T: O(n)~O(2n), S: O(min(n, charset))

// sliding-window + hash-map, T: O(n) one pass, S: O(min(n, charset))

#include <string>
#include <unordered_map>
#include <algorithm> // std::max

class Solution {
public:
    int lengthOfLongestSubstring(std::string s) {
        std::unordered_map<char, int> lastSeen;
        lastSeen.reserve(s.length());

        int left = 0;
        int maxLen = 0;
        for (int right = 0; right < static_cast<int>(s.length()); right++) {
            auto it = lastSeen.find(s[right]);
            if (it != lastSeen.end() && it->second >= left) {
                left = it->second + 1;
            }
            lastSeen[s[right]] = right;
            maxLen = std::max(maxLen, right - left + 1);
        }
        return maxLen;
    }
};

// fixed-array + direct jump, T: O(n), S: O(1)

#include <string>
#include <cstring> // std::memset
#include <algorithm> // std::max

class Solution {
public:
    int lengthOfLongestSubstring(std::string s) {
        constexpr int R = 128;
        int lastSeen[R];
        std::memset(lastSeen, -1, sizeof(lastSeen));

        int left = 0;
        int maxLen = 0;
        for (int right = 0; right < static_cast<int>(s.length()); right++) {
            unsigned char c = static_cast<unsigned char>(s[right]);
            if (lastSeen[c] >= left) { left = lastSeen[c] + 1; }
            lastSeen[c] = right;
            maxLen = std::max(maxLen, right - left + 1);
        }
        return maxLen;
    }
};

// >= left check: jump left to lastSeen[c]+1 conditionally
