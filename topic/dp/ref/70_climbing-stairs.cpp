// LC 70 Climbing Stairs: you climb n steps taking 1 or 2 at a time. How many
// distinct ways reach the top?
#include <iostream>
#include <string>
using std::cout;
using std::string;

// ways(i) = ways(i-1) + ways(i-2), keeping only two values. T: O(n), S: O(1).
int climbStairs(int n) {
    int two = 1, one = 1;   // ways to reach step i-2 and step i-1 (steps 0 and 1 start)
    for (int i = 2; i <= n; i++) {
        int cur = two + one;
        two = one;
        one = cur;
    }
    return one;
}

void test(const string& label, int n, int want) {
    cout << label << (climbStairs(n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one", 1, 1);
    test("four", 4, 5);
    test("ten", 10, 89);
}
