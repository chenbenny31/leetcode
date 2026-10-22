// sliding-window + char-index-array, T: O(m + n), S: O(R)

#include <string>
#include <vector>

class Solution {
public:
    bool checkInclusion(std::string s1, std::string s2) {
        if (s1.size() > s2.size()) { return false; }
        constexpr int R = 26;
        constexpr char BASE = 'a';

        std::vector<int> freq(R, 0); // char: remain cnt need towards s1
        for (char c : s1) { freq[c - BASE]++; }

        int m = static_cast<int>(s1.size());
        int n = static_cast<int>(s2.size());

        int have = 0;
        int left = 0;
        for (int right = 0; right < n; right++) {
            int r = s2[right] - BASE;
            if (freq[r] > 0) { have++; }
            freq[r]--;

            if (right - left + 1 > m) {
                int l = s2[left] - BASE;
                left++;
                freq[l]++;
                if (freq[l] > 0) { have--; }
            }
            if (have == m) { return true; }
        }
        return false;
    }
};
