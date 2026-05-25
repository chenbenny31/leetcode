// sliding-window + flat-array, T: O(n + m), S: O(1) stack-alloc

#include <string>
#include <cstring> // std::memset
#include <climits> // INT_MAX

class Solution {
public:
    std::string minWindow(std::string s, std::string t) {
        if (s.size() < t.size()) { return ""; }
        constexpr int R = 128;
        int freq[R];
        std::memset(freq, 0, sizeof(freq));

        int need = static_cast<int>(t.size());
        int have = 0;
        for (char c : t) { freq[static_cast<unsigned char>(c)]++; }

        int left = 0;
        int best_left = 0;
        int best_len = INT_MAX;

        for (int right = 0; right < static_cast<int>(s.size()); right++) {
            int rc = static_cast<unsigned char>(s[right]);
            if (freq[rc] > 0) { have++; }
            freq[rc]--;

            while (have == need) {
                if (right - left + 1 < best_len) {
                    best_len = right - left + 1;
                    best_left = left;
                }
                int lc = static_cast<unsigned char>(s[left]);
                left++;
                freq[lc]++;
                if (freq[lc] > 0) { have--; }
            }
        }
        return best_len == INT_MAX ? "" : s.substr(best_left, best_len);
    }
};

// freq[rc] > 0 fires only when char still needed; freq[lc] > 0 after incre fires
// cache freq[128] = 512 bytes
// dup chars of t: freq[c] use 0 guard, need = t.size() counts total
