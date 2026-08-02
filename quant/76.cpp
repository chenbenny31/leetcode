// sliding-window + char-index-array

#include <string>
#include <climits> // INT_MAX

class Solution {
public:
    std::string minWindow(std::string s, std::string t) {
        if (s.size() < t.size()) { return ""; }
        constexpr int R = 128;
        std::vector<int> freq(R, 0); // freq of chars in t remain to be matched by s

        int need = static_cast<int>(t.size());
        int have = 0;
        for (char c : t) { freq[static_cast<unsigned char>(c)]++; }

        int left = 0;
        int start = 0; int len = INT_MAX; // min-window of matched substring
        for (int right = 0; right < static_cast<int>(s.size()); right++) {
            int rc = static_cast<unsigned char>(s[right]);
            if (freq[rc] > 0) { have++; }
            freq[rc]--;

            while (have == need) {
                if (right - left + 1 < len) {
                    len = right - left + 1;
                    start = left;
                }

                int lc = static_cast<unsigned char>(s[left]);
                left++;
                freq[lc]++;
                if (freq[lc] > 0) { have--; }
            }
        }
        return len == INT_MAX ? "" : s.substr(start, len);
    }
};
