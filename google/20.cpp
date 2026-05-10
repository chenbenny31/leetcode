// stack, T: O(n), S: O(n)

#include <string>
#include <cstring> // std:memset

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

        char stk[10001]; // flat stack, bounded by s.size()
        int top = -1;

        for (char c : s) {
            if (c == '(' || c == '[' || c == '{') {
                stk[++top] = c;
            } else {
                if (top == -1 || stk[top] != match[static_cast<unsigned char>(c)]) { return false; }
                --top;
            }
        }
        return top == -1;
    }
};

// flat stack over std::stack: std::stack use std::deque which is heap allocation, char stk[10001] is stack-allocated fit in L1
// match table: enables branchless lookup, single indexed load over three branches, explicit jump table for compiler
// early exit on odd length
// unsigned chat casting: char is signed, values > 127 produces negative indices, unsigend chat ensure [0, 127]
// cache match[128] is 128 bytes: fit in one to two cache lines in L1, stk[10001] is 10KB fit in L1 too.
// return top == -1: check if unmatches open brackets remain on stack
// O(1) space only for single brackets, track count by integer
