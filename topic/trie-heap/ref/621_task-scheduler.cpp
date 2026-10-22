// LC 621 Task Scheduler: tasks are letters, each takes one unit; two runs of the same
// letter need at least n idle/other units between them. Return the minimum total units.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Greedy by frequency: the most frequent task fixes (maxf - 1) frames of width n + 1,
// plus a last row holding every task tied for max. Other tasks fill idle slots; if they
// overflow the frames, no idle is needed and the answer is tasks.size().
// T: O(T), S: O(1).
int leastInterval(vector<char>& tasks, int n) {
    vector<int> cnt(26, 0);   // cnt[c] = occurrences of task 'A' + c
    for (char t : tasks) { cnt[t - 'A']++; }
    int maxf = 0;
    for (int f : cnt) { maxf = max(maxf, f); }
    int ties = 0;             // tasks that occur maxf times
    for (int f : cnt) {
        if (f == maxf) { ties++; }
    }
    int frames = (maxf - 1) * (n + 1) + ties;
    return max((int)tasks.size(), frames);
}

void test(const string& label, vector<char> tasks, int n, int want) {
    cout << label << (leastInterval(tasks, n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("idle-needed", {'A', 'A', 'A', 'B'}, 2, 7);
    test("no-cooldown", {'A', 'A', 'B'}, 0, 3);
    test("ties-and-fill", {'A', 'A', 'B', 'B', 'C', 'D'}, 1, 6);
    test("single", {'Z'}, 5, 1);
}
