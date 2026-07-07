// sorting, T: O(nlogn), S: O(1)

// char-counting array, T: O(n), S: O(1)

#include <string>

class Solution {
public:
    bool isAnagram(std::string s, std::string t) {
        if (s.length() != t.length()) { return false; }

        constexpr int R = 26;
        constexpr char BASE = 'a';
        const int n = static_cast<int>(s.length());

        int freq[R] = {};
        for (int i = 0; i < n; i++) {
            freq[s[i] - BASE]++;
        }
        for (int i = 0; i < n; i++) {
            freq[t[i] - BASE]--;
            if (freq[t[i] - BASE] < 0) { return false; }
        }
        return true;
    }
};

// follow-up Unicode extension: char32_t as key for unodered_map
