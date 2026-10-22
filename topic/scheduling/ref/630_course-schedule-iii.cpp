// LC 630 Course Schedule III: course i takes duration[i] days and must finish by
// lastDay[i]; courses are taken one at a time. Return the maximum number of courses.
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::sort;

// Sort by deadline and take every course; whenever the total overshoots the current
// deadline, evict the longest course taken so far (frees the most time for one slot).
// T: O(n log n), S: O(n).
int scheduleCourse(vector<vector<int>>& courses) {
    sort(courses.begin(), courses.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    priority_queue<int> taken;   // durations of the courses currently kept
    int total = 0;               // sum of the kept durations
    for (const auto& c : courses) {
        taken.push(c[0]);
        total += c[0];
        if (total > c[1]) {
            total -= taken.top();
            taken.pop();
        }
    }
    return taken.size();
}

void test(const string& label, vector<vector<int>> courses, int want) {
    cout << label << (scheduleCourse(courses) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("all-fit", {{2, 5}, {1, 2}}, 2);
    test("evict-earlier", {{5, 5}, {2, 6}, {2, 6}}, 2);
    test("too-long", {{5, 3}}, 0);
}
