// sliding-window + flat-array, T: O(n + m), S: O(1) stack-alloc

#include <string>
#include <cstring> // std::memset

class Solution {
public:
    bool checkInclusion(std::string s1, std::string s2) {
        if (s1.size() > s2.size()) { return false; }

        constexpr int R = 26;
        constexpr char BASE = 'a';

        int freq[R];
        std::memset(freq, 0, sizeof(freq));
        for (char c : s1) { freq[c - BASE]++; }

        int need = static_cast<int>(s1.size());
        int have = 0;
        int left = 0;
        int wsize = static_cast<int>(s1.size());

        for (int right = 0; right < static_cast<int>(s2.size()); right++) {
            int rc = s2[right] - BASE;
            if (freq[rc] > 0) { have++; }
            freq[rc]--;
            
            if (right - left + 1 > wsize) {
                int lc = s2[left] - BASE;
                left++;
                freq[lc]++;
                if (freq[lc] > 0) { have--; }
            }

            if (have == need) { return true; }
        }
        return false;
    }
};

//
