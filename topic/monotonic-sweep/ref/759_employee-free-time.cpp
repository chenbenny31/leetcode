// LC 759 Employee Free Time: schedule[i] is employee i's sorted, disjoint working
// intervals [start, end). Return the finite intervals in which every employee is free,
// in order. Intervals that only touch ([1, 2] and [2, 3]) leave no free time.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::max;
using std::sort;
using std::string;
using std::vector;

struct Interval {
    int start, end;
    Interval(int s, int e) : start(s), end(e) {}
};

// merge all intervals by start and report each gap after the running max end.
// T: O(N log N) for N intervals, S: O(N).
vector<Interval> employeeFreeTime(const vector<vector<Interval>>& schedule) {
    vector<Interval> all;   // everyone's intervals together
    for (const auto& emp : schedule) {
        for (const Interval& iv : emp) { all.push_back(iv); }
    }
    sort(all.begin(), all.end(), [](const Interval& a, const Interval& b) {
        return a.start < b.start;
    });
    vector<Interval> free;
    int reach = all[0].end;   // latest end seen: someone is busy until then
    for (const Interval& iv : all) {
        if (iv.start > reach) { free.push_back(Interval(reach, iv.start)); }
        reach = max(reach, iv.end);
    }
    return free;
}

void test(const string& label, const vector<vector<Interval>>& schedule,
          const vector<vector<int>>& want) {
    vector<vector<int>> got;
    for (const Interval& iv : employeeFreeTime(schedule)) {
        got.push_back({iv.start, iv.end});
    }
    if (got == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("two-gaps", {{{1, 3}, {6, 7}}, {{2, 4}}, {{2, 5}, {9, 12}}}, {{5, 6}, {7, 9}});
    test("touching", {{{1, 2}}, {{2, 3}}}, {});
    test("nested", {{{1, 10}}, {{2, 3}, {4, 5}}}, {});
    test("one-employee", {{{1, 2}, {5, 6}}}, {{2, 5}});
}
