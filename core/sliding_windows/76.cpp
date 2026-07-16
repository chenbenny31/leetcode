// sliding-window + hash-map, T: O(m+n), S: O(m+n)

// sliding-window + fixed-array, S: O(m+n), S: O(1)

#include <string>
#include <climits> // INT_MAX

class Solution {
public:
    std::string minWindow(std::string s, std::string t) {
        if (s.size() < t.size()) { return ""; }

        constexpr int R = 128;
        int freq[R] = {};
        std::memset(freq, 0, sizeof(freq));

        int need = static_cast<int>(t.size());
        int have = 0;
        for (char c : t) { freq[static_cast<unsigned char>(c)]++; }

        int left = 0;
        int bestLeft = 0;
        int bestLen = INT_MAX;
        for (int right = 0; right < static_cast<int>(s.size()); right++) {
            int rc = static_cast<unsigned char>(s[right]);
            if (freq[rc] > 0) { have++; }
            freq[rc]--;

            while (have == need) {
                if (right - left + 1 < bestLen) {
                    bestLen = right - left + 1;
                    bestLeft = left;
                }
                int lc = static_cast<unsigned char>(s[left]);
                left++;
                freq[lc]++;
                if (freq[lc] > 0) { have--; }
            }
        }
        return bestLen == INT_MAX ? "" : s.substr(bestLeft, bestLen);
    }
};

// head/need invariant: freq[c] == need[c] - window[c], head only reacts when freq[c] cross 0
