// monotonic-stack, T: O(n), S: O(n)

#include <vector>
#include <cstdlib> // std::abs

class Solution {
public:
    std::vector<int> asteroidCollision(std::vector<int>& asteroids) {
        int n = static_cast<int>(asteroids.size());
        // constexpr int R = 20001;
        int stk[n]; // flat-stack
        int top = -1;

        for (int a : asteroids) {
            bool destroyed = false;
            while (top != -1 && a < 0 && stk[top] > 0) {
                if (stk[top] < std::abs(a)) {
                    --top; // stack top destroyed
                } else if (stk[top] == std::abs(a)) {
                    --top; // both destroyed
                    destroyed = true;
                    break;
                } else {
                    destroyed = true; // incoming destroyed
                    break;
                }
            }
            if (!destroyed) { stk[++top] = a; }
        }

        std::vector<int> out(top + 1);
        for (int i = 0; i <= top; ++i) { out[i] = stk[i]; }
        return out;
    }
};

// collision condition: a < 0 && stk[top] > 0
// destoryed flag: help tracks wether to push
// variable-length array check: GCC extension for int stk[n], C++17 use vector.reserve(asteroids.size())
