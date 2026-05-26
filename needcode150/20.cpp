// flat-stack, T: O(n), S: O(n)

#include <vector>
#include <string>
#include <cstring> // std::memset

class Solution {
public:
    bool isValid(std::string s) {
        if (s.size() % 2 != 0) { return false; }

        constexpr int R = 128;
        char match[R];
        std::memset(match, 0, sizeof(match));
        match[static_cast<unsigned char>(')')] = '(';
        match[static_cast<unsigned char>(']')] = '[';
        match[static_cast<unsigned char>('}')] = '{';

        constexpr int N = 10001;
        char stk[N];
        std::vector<char> stk;
        stk.reserve(s.size());
        int top = -1;

        for (char c : s) {
            if (c == '(' || c == '[' || c == '{') {
                top++;
                stk[top] = c;
            } else {
                if (top == -1 || stk[top] != match[static_cast<unsigned char>(c)]) { return false; }
                top--;
            }
        }
        return top == -1;
    }
};

// match table: single indexed load replaces three branches
// flat stack array: stack-alloc, fits in L1
// early exist on odd len
// unsigned char casting guard vals > 127 produce neg indices
// cache match[128] = 128 bytes, L1 resident
// extend to O(1) space: only for single bracket, count opens as int, mixed type need stack to track
