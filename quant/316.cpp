// stack

#include <string>
#include <vector>
#include <cstdint>

class Solution {
public:
    string removeDuplicateLetters(string s) {
        const int n = static_cast<int>(s.length());
        constexpr int R = 26;
        constexpr int BASE = 'a';

        std::vector<int> lastIdx(R, 0); // lastIdx prevents deleting of the last char
        std::vector<uint8_t> used(R, 0);
        for (int i = 0; i < n; i++) {
            lastIdx[s[i] - BASE] = i;
        }

        std::string stk;
        stk.reserve(R);

        for (int i = 0; i < n; i++) {
            if (used[s[i] - BASE]) { continue; }

            while (!stk.empty() && s[i] < stk.back() && lastIdx[stk.back() - BASE] > i) {
                used[stk.back() - BASE] = 0;
                stk.pop_back();
            }
            stk.push_back(s[i]);
            used[s[i] - BASE] = 1;
        }
        return stk;
    }
};
