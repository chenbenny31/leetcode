// LC 739 Daily Temperatures (Medium) — monotonic decreasing stack
// For each day, return the number of days until a strictly warmer temperature, 0 if
// there is none.
// Clarify: warmer is strictly greater (an equal day does not count); no warmer day
//   gives 0; answer is a distance in days, not a temperature or an index.

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// Core idea: keep the days still waiting for a warmer one on a stack; day i pops and
// answers every colder day on top, then waits itself. Each day is pushed and popped at
// most once. T: O(n), S: O(n).
vector<int> dailyTemperatures(const vector<int>& temps) {
    int n = temps.size();
    // wait[i] = days until warmer than day i, 0 if never
    vector<int> wait(n, 0);
    // unresolved days, temps non-increasing bottom to top
    vector<int> stk;
    for (int i = 0; i < n; i++) {
        // strict <: an equal day is not warmer
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
    return 0;
}

// follow-up (ideas only):
// - next greater for a subset of values (LC 496) -> one stack pass over nums2 fills a map
//   value -> next greater; nums1 only looks up, -1 when the value has no entry
// - circular array (LC 503) -> walk the array twice with i % n; push only first-pass
//   indices, the second pass only resolves
// - stock span, prices streamed in (LC 901) -> stack of (price, span); pop while the top
//   price <= today's and add the popped span, not 1
// - people visible to the left (LC 1944 mirrored) -> decreasing stack; person i sees
//   every shorter one it pops plus the taller top left after the pops
// - equal heights in that problem -> an equal top is seen, popped, and blocks the taller
//   one under it
// - next / previous smaller -> flip the comparison (pop while the top is larger); the
//   stack then holds rising values
// - car fleet (LC 853) -> sort by position descending, stack of arrival times; a car
//   whose time is <= the fleet ahead (the top) merges into it, else it is a new fleet
