// mono-stack

#include <string>
#include <algorithm> // std::min

class Solution {
public:
    string removeKdigits(string num, int k) {
        std::string stk; // store incre char
        stk.reserve(num.length());

        for (char c : num) {
            while (k > 0 && !stk.empty() && c < stk.back()) {
                stk.pop_back();
                k--;
            }
            stk.push_back(c);
        }

        for (int i = 0; i < k; i++) { stk.pop_back(); }

        int start = 0;
        while (start < static_cast<int>(stk.size()) - 1 // prevents "0" skip
                && stk[start] == '0') {
            start++;
        }
        return stk.empty() ? "0" : stk.substr(start);
    }
};
