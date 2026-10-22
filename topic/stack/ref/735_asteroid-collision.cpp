// LC 735 Asteroid Collision: asteroids move along a line; the sign is the direction, the
// absolute value the size. Same-direction asteroids never meet. On a collision the
// smaller explodes, equal sizes both explode. Return the survivors in order.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Stack of survivors; a left-mover collides only with right-movers on top.
// T: O(n), S: O(n).
vector<int> asteroidCollision(const vector<int>& asteroids) {
    vector<int> stk;   // survivors so far, in order
    for (int a : asteroids) {
        bool alive = true;
        while (alive && a < 0 && !stk.empty() && stk.back() > 0) {
            if (stk.back() < -a) {
                stk.pop_back();
            } else {
                // equal: both explode; bigger top: a explodes
                if (stk.back() == -a) { stk.pop_back(); }
                alive = false;
            }
        }
        if (alive) { stk.push_back(a); }
    }
    return stk;
}

void test(const string& label, const vector<int>& asteroids, const vector<int>& want) {
    if (asteroidCollision(asteroids) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("cascade", {1, 2, -3}, {-3});
    test("equal-both-explode", {4, -4}, {});
    test("diverging", {-2, -1, 1, 2}, {-2, -1, 1, 2});
    test("bigger-right-survives", {3, -1, -2}, {3});
}
