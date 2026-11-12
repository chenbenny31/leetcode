// LC 739 Daily Temperatures (Medium). Pattern: monotonic decreasing stack.
// For each day, the number of days until a strictly warmer temperature; 0 if none.
//
// Clarify:
// - warmer = strictly greater -> an equal day does not count
// - no warmer day later -> 0
// - return a distance in days (j - i), not the warmer temperature or its index
// - n up to 1e5, temps 30..100 -> checking every later day is O(n^2), too slow
// Idea: keep the days still waiting for a warmer one on a stack; their temps never rise
//   going up the stack. Day i pops (and answers) every colder day on top, then waits
//   itself. Each day is pushed and popped at most once: O(n).
// Pitfalls:
// - push indices, not temps: the answer is a distance
// - pop while temps[top] < temps[i] (strict); with <= an equal day gets an answer
// - while, not if: one warm day can answer many colder days
// - check !stk.empty() before stk.back()
// - days still on the stack at the end keep 0, so wait starts at 0
// Tests to write: mixed; strictly decreasing -> all 0; equal days are not warmer.

#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

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
// Pitfall: with equal heights this over-counts; see the follow-up for the fix.
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

// LC 901 (streaming): next(price) returns today's span = consecutive days up to today
// with price <= today's. Stack of (price, span) with prices strictly falling bottom to
// top; a day with price <= today is absorbed into today's span. T: O(1) amortized per
// call, S: O(n).
// Pitfall: pop on <= (an equal price belongs to the span), and add the popped span,
//   not 1.
class StockSpanner {
    // (price, span) of days not absorbed yet, prices strictly falling bottom to top
    vector<pair<int, int>> stk;

public:
    int next(int price) {
        int span = 1;
        while (!stk.empty() && stk.back().first <= price) {
            span += stk.back().second;
            stk.pop_back();
        }
        stk.push_back({price, span});
        return span;
    }
};

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

void testSpan(const string& label, const vector<int>& prices, const vector<int>& want) {
    StockSpanner spanner;
    vector<int> spans;
    for (int price : prices) { spans.push_back(spanner.next(price)); }
    cout << label << (spans == want ? " PASS\n" : " FAIL\n");
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
    testSpan("span-absorb", {5, 3, 4, 6}, {1, 1, 2, 4});
    testSpan("span-equal", {2, 2}, {1, 2});
    testSpan("span-falling", {9, 8, 7}, {1, 1, 1});
}

// follow-up: circular array (LC 503) -> nextGreaterCircular above
// follow-up: people visible to the left (LC 1944, Google ×3) -> countVisibleLeft above
// follow-up: equal heights in countVisibleLeft -> an equal top is seen, then hidden for
//   good, and it blocks the taller one under it; replace if (!stk.empty()) seen[i]++:
//     bool equalTop = !stk.empty() && heights[stk.back()] == heights[i];
//     if (equalTop) { stk.pop_back(); }
//     if (equalTop || !stk.empty()) { seen[i]++; }
// follow-up: stock span, prices streamed in (LC 901) -> StockSpanner above
// follow-up: next greater for a subset of values (LC 496) -> run the same stack over
//   nums2, and when x pops for cur, record nextOf[x] = cur (unordered_map<int, int>);
//   answer each query with nextOf.count(q) ? nextOf[q] : -1
// follow-up: next / previous smaller -> flip the comparison (pop while
//   temps[stk.back()] > temps[i]); the stack then holds rising values
