// greedy + monotonic-stack, T: O(n), S: O(1)

#include <string>
#include <cstring> // std::memset
#include <cctype> // std::isalpha

class Solution {
public:
    std::string removeDuplicateLetters(std::string s) {
        constexpr int R = 26;
        constexpr char BASE = 'a';

        int freq[R];
        bool used[R];
        std::memset(freq, 0, sizeof(freq));
        std::memset(used, 0, sizeof(used));

        for (char c : s) { freq[c - BASE]++; }

        std::string stk;
        stk.reserve(R);

        for (char c : s) {
            int ci = c - BASE;
            freq[ci]--;
            
            if (used[ci]) { continue; }
            
            while (!stk.empty() && c < stk.back() && freq[stk.back() - BASE] > 0) {
                used[stk.back() - BASE] = false;
                stk.pop_back();
            }

            stk.push_back(c);
            used[ci] = true;
        }
        return stk;
    }
};

// std::string as stack, with reserve
// freq[ci]-- before used check: ensure occurrence count correctly
// freq[c] > 0 to guarantee safe pop
