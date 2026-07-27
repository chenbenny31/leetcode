// stack

#include <vector>
#include <stack>
#include <unordered_map>

class Solution {
private:
    static constexpr char match(char c) {
        switch(c) {
        case ')': return '(';
        case ']': return '[';
        case '}': return '{';
        default:  return '\0';
        }
    }
public:
    bool isValid(string s) {
        std::vector<char> buf;
        buf.reserve(s.length());
        std::stack<char, std::vector<char>> stk(std::move(buf));
         
        for (char c : s) {
            if (c == '(' || c == '[' || c == '{') {
                stk.push(c);
            } else {
                if (stk.empty() || stk.top() != match(c))  { return false; }
                else { stk.pop(); }
            }
        }
        return stk.empty();
    }
};
