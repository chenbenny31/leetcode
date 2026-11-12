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

// LC 503: walk the array twice (i % n) so later indices see the start; push only
// first-pass indices, the second pass only resolves. T: O(n), S: O(n).
vector<int> nextGreaterCircular(const vector<int>& nums) {
    int n = nums.size();
    // nextGreater[i] = first larger value after i going around, -1 if none
    vector<int> nextGreater(n, -1);
    // unresolved indices, values non-increasing bottom to top
    vector<int> stk;
    for (int i = 0; i < 2 * n; i++) {
        int cur = nums[i % n];
        while (!stk.empty() && nums[stk.back()] < cur) {
            nextGreater[stk.back()] = cur;
            stk.pop_back();
        }
        if (i < n) {
            stk.push_back(i);
        }
    }
    return nextGreater;
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

void testCircular(const string& label, const vector<int>& nums,
                  const vector<int>& want) {
    cout << label << (nextGreaterCircular(nums) == want ? " PASS\n" : " FAIL\n");
}

void testVisible(const string& label, const vector<int>& heights,
                 const vector<int>& want) {
    cout << label << (countVisibleLeft(heights) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {60, 50, 70, 55, 65}, {2, 1, 0, 1, 0});
    test("decreasing", {90, 60, 30}, {0, 0, 0});
    test("equal-not-warmer", {50, 50, 51}, {2, 1, 0});
    testCircular("circ-wrap-around", {3, 1, 2}, {-1, 2, 3});
    testCircular("circ-max-in-middle", {2, 4, 1}, {4, -1, 2});
    testCircular("circ-all-equal", {5, 5}, {-1, -1});
    testVisible("visible-valley", {4, 1, 3}, {0, 1, 2});
    testVisible("visible-blocked", {2, 1, 3, 4}, {0, 1, 2, 1});
    testVisible("visible-increasing", {1, 2, 3}, {0, 1, 1});
    testVisible("visible-decreasing", {3, 2, 1}, {0, 1, 1});
}

// follow-up: stock span (LC 901) -> stack of (price, span); pop and add spans while <=
