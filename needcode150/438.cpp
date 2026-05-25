// sliding-window + flat-array, T: O(n + m), S: O(1) stack-alloc

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
        int wsize = static_cast<int>(p.size());

        for (int right = 0; right < static_cast<int>(s.size()); right++) {
            int rc = s[right] - BASE;
            if (freq[rc] > 0) { have++; }
            freq[rc]--;

            if (right - left + 1 > wsize) {
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

// out.reserve for worst case: entire s is one long anagram, p: n - m + 1
// extend to p is larger than s: early return guard returns empty vec immediately
