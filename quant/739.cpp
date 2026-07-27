// mono-stack

#include <stack>
#include <vector>

class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        int n = static_cast<int>(temperatures.size());
        std::vector<int> buf;
        buf.reserve(n);
        std::stack<int, std::vector<int>> stk(std::move(buf)); // store idx with decre temp

        std::vector<int> out(n, 0);
        for (int i = 0; i < n; i++) {
            int curTemp = temperatures[i];
            while (!stk.empty() && curTemp > temperatures[stk.top()]) {
                out[stk.top()] = i - stk.top();
                stk.pop();
            }
            stk.push(i);
        }
        return out;
    }
};
