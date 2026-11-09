// LC 739 Daily Temperatures (Medium). Pattern: monotonic decreasing stack.
// For each day, the number of days until a strictly warmer temperature; 0 if none.

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// Stack of days still waiting; a warmer day resolves every colder day on top.
// T: O(n), S: O(n).
vector<int> dailyTemperatures(const vector<int>& temps) {
    int n = temps.size();
    // wait[i] = days until warmer than day i, 0 if never
    vector<int> wait(n, 0);
    // unresolved days, temps non-increasing bottom to top
    vector<int> stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && temps[stk.back()] < temps[i]) {
            int colderDay = stk.back();
            stk.pop_back();
            wait[colderDay] = i - colderDay;
        }
        stk.push_back(i);
    }
    return wait;
}

void test(const string& label, const vector<int>& temps, const vector<int>& want) {
    cout << label << (dailyTemperatures(temps) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {60, 50, 70, 55, 65}, {2, 1, 0, 1, 0});
    test("decreasing", {90, 60, 30}, {0, 0, 0});
    test("equal-not-warmer", {50, 50, 51}, {2, 1, 0});
}

// follow-up: circular next greater (LC 503) -> loop i over 2n with i % n, push only i < n
// follow-up: stock span (LC 901) -> stack of (price, span); pop and add spans while <=
