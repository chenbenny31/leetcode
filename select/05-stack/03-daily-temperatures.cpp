// LC 739 Daily Temperatures (Medium) — monotonic decreasing stack
// for each day return num of days until a strictly warmer temp, 0 if none
//
// clarify:
// warmer -> strictly greater; an equal day does not count
// no warmer day -> 0
// answer -> distance in days, not a temp or an idx
// circular? -> follow-up (LC 503)

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// core idea: stack of days still waiting; day i pops and answers every colder day on top
// T: O(n), S: O(n) — each day pushed and popped at most once
vector<int> dailyTemperatures(const vector<int>& temps) {
    int n = temps.size();
    // wait[i] = days until warmer than day i, 0 if never
    vector<int> wait(n, 0);
    // stk = unresolved days, temps non-increasing bottom to top
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

// follow-up:
// next greater for a subset (LC 496) -> one stack pass over nums2 fills map value -> next
// circular array (LC 503) -> walk twice w/ i % n; push only first-pass idx
// stock span (LC 901) -> stack of (price, span); pop while top <= today, add its span
// people visible to the left (LC 1944 mirrored) -> every shorter one popped + taller top
// equal heights there -> an equal top is seen, popped, and blocks the taller one under it
// next / previous smaller -> flip the comparison; stack then holds rising values
// car fleet (LC 853) -> sort by position desc, stack of arrival times; time <= top merges
