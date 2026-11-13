// 25 · Daily Temperatures (Monotonic Stack) — LC 739, 496, 503, 1944
// Mother: select/05-stack/03-daily-temperatures.cpp
// (1) LC 739: for each day, days until a strictly warmer temp; 0 if none
// (2) visible people to the left: for each person, cnt of people seen on the left
// reported: visible people to the left x3 (countVisibleLeft); temperature monitor
//
// clarify:
// strictly warmer or >=? -> strictly
// equal heights in (2) -> assumed distinct; equal is a follow-up
// n up to 1e5 -> no O(n^2)

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// (1) days until warmer (LC 739)
// core idea: stack of waiting days; day i pops (answers) every colder top, then waits
// T: O(n), S: O(n)
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

// (2) visible people to the left
// core idea: i sees j < i iff everyone between is shorter than both
// a shorter person popped by i is seen, then hidden for good; the taller top left is seen
// T: O(n), S: O(n)
vector<int> countVisibleLeft(const vector<int>& heights) {
    int n = heights.size();
    // seen[i] = people person i sees to the left
    vector<int> seen(n, 0);
    // people not yet hidden by a later taller one; heights decreasing bottom to top
    vector<int> stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && heights[stk.back()] < heights[i]) {
            seen[i]++;
            stk.pop_back();
        }
        // first taller person to the left blocks the rest but is seen
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

// follow-up:
// visible w/ equal heights -> pop an equal top, count it, then stop: it blocks the rest
// next greater for a subset (LC 496) -> stack over nums2 fills map value -> next greater
// circular array (LC 503) -> walk 2n w/ i % n, push only first-pass indices
// stock span, streamed (LC 901) -> stack of (price, span); pop on <=, add popped spans
// next / previous smaller -> flip the comparison
// sliding-window max (temp monitor) -> deque of idx, decreasing values; pop stale front
