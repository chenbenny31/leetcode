// flat-stack, T: O(n), S: O(n)

#include <vector>
#include <string>

class Solution {
public:
    int evalRPN(std::vector<std::string>& token) {
        constexpr int N = 10001;
        long long stk[N];
        int top = -1;

        for (auto& tok : token) {
            if (tok == "+" || tok == "-" || tok == "*" || tok == "/") {
                long long b = stk[top]; top--;
                long long a = stk[top]; top--;
                long long res;
                if (tok == "+") { res = a + b; }
                else if (tok == "-") { res = a - b; }
                else if (tok == "*") { res = a * b; }
                else { res = a / b; }
                top++;
                stk[top] = res;
            } else {
                top++;
                stk[top] = std::stoll(tok);
            }
        }
        return static_cast<int>(stk[top]);
    }
};

// long long for intermediate val: int prod can overlof, e.g. INT_MAX * INT_MAX
// flat stack array: entire fits in L1
// pop order: b before a since stack is LIFO
// cache stk within L1, tokens vec of strs - heap and ptr chasing, can be pre-tokenized into flat struct
// stoll not stoi
// RPN in prac: JVM, CPython, PostScript, any stack-based eval
