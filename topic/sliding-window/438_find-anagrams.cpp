// sliding-window + char-index-array, T: O(m+n), S: O(R)

#include <vector>
#include <string>

class Solution {
public:
    std::vector<int> findAnagrams(std::string s, std::string p) {
        std::vector<int> out;
        if (s.size() < p.size()) { return out; }

        constexpr int R = 26;
        constexpr char BASE = 'a';
        std::vector<int> freq(R, 0); // cnts of char in win
        for (char c : p) { freq[c - BASE]++; }

        int len = static_cast<int>(p.size());
        int n = static_cast<int>(s.size());

        int have = 0;
        int left = 0;
        for (int right = 0; right < n; right++) {
            int r = s[right] - BASE;
            if (freq[r] > 0) { have++; }
            freq[r]--;

            if (right - left + 1 > len) {
                int l = s[left] - BASE;
                left++;
                freq[l]++;
                if (freq[l] > 0) { have--; }
            }

            if (have == len) { out.push_back(left); }
        }
        return out;
    }
};
