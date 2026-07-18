// sliding-window + flat-array, T: O(m+n), S: O(1)

#include <vector>
#include <string>
#include <cstring> // std::memset

class Solution {
public:
    std::vector<int> findAnagrams(std::string s, std::string p) {
        std::vector<int> out;
        if (s.size() < p.size()) { return out; }
        constexpr int R = 26;
        constexpr char BASE = 'a';

        int freq[R];
        std::memset(freq, 0, sizeof(freq));
        for (char c : p) { freq[c - BASE]++; }

        int need = static_cast<int>(p.size());
        int have = 0;
        int left = 0;
        int winSize = static_cast<int>(p.size());

        for (int right = 0; right < static_cast<int>(s.size()); right++) {
            int rc = s[right] - BASE;
            if (freq[rc] > 0) { have++; }
            freq[rc]--;

            if (right - left + 1 > winSize) {
                int lc = s[left] - BASE;
                left++;
                freq[lc]++;
                if (freq[lc] > 0) { have--; }
            }

            if (have == need) { out.push_back(left); }
        }
        return out;
    }
};

// have/need: total filled occurrence char
