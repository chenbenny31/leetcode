// 23 · Daily Temperatures (Monotonic Stack) — LC 739, 496, 503, 1944
// Mother problem: select/05-stack/03-daily-temperatures.cpp
//
// Google reports: visible people to the left ×3 (`countVisibleLeft`), temperature
// monitor (81) sliding max
//
// LC 739: for each day, the number of days until a strictly warmer temperature; 0 if
// none. n up to 1e5 → O(n^2) too slow.
//
// Core idea
// - Stack of day indices still waiting; temps never rise going up the stack.
// - Day i pops (and answers) every strictly colder day on top, then waits itself.
// - Each index is pushed and popped once: T O(n), S O(n).
//
// Follow-ups
// - people visible to the left (LC 1944 mirrored, Google ×3) → `countVisibleLeft` below;
//   equal heights → pop an equal top and count it, and then it blocks the one below
// - next greater for a subset (LC 496) → same stack over nums2 fills a map value → next
//   greater, then look up each nums1 value (-1 if missing)
// - circular array (LC 503) → walk 2n with i % n, push only first-pass indices
// - stock span, streamed (LC 901) → stack of (price, span), pop on <= and add the popped
//   spans
// - next / previous smaller → flip the comparison

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

// Google (LC 1944 mirrored): person i sees j < i iff everyone strictly between is
// shorter than both; return how many each person sees to the left. Assumes distinct
// heights. A shorter person popped from the stack is seen and then hidden for good; the
// taller top left after popping is seen too. T: O(n), S: O(n).
vector<int> countVisibleLeft(const vector<int>& heights) {
    int n = heights.size();
    // seen[i] = people person i sees to the left
    vector<int> seen(n, 0);
    // people not yet hidden by a taller one after them, heights decreasing bottom to top
    vector<int> stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && heights[stk.back()] < heights[i]) {
            seen[i]++;
            stk.pop_back();
        }
        // the first taller person to the left blocks the rest but is seen
        if (!stk.empty()) {
            seen[i]++;
        }
        stk.push_back(i);
    }
    return seen;
}

void test(const string& label, const vector<int>& temps, const vector<int>& want) {
    cout << label << (dailyTemperatures(temps) == want ? " PASS\n" : " FAIL\n");
}

void testVisible(const string& label, const vector<int>& heights,
                 const vector<int>& want) {
    cout << label << (countVisibleLeft(heights) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {60, 50, 70, 55, 65}, {2, 1, 0, 1, 0});
    test("decreasing", {90, 60, 30}, {0, 0, 0});
    test("equal-not-warmer", {50, 50, 51}, {2, 1, 0});
    testVisible("visible-valley", {4, 1, 3}, {0, 1, 2});
    testVisible("visible-blocked", {2, 1, 3, 4}, {0, 1, 2, 1});
    testVisible("visible-increasing", {1, 2, 3}, {0, 1, 1});
}
