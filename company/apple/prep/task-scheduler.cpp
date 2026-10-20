// tasks are letters A-Z, one unit each, return the min total units
// two runs of same letter need at least n units cool down

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::queue;
using std::priority_queue;
using std::max;
using std::max_element;
using std::count;

// simulate, T: O(Tlog26), S: O(26)
int leastInterval(const vector<char>& tasks, int n) {
    vector<int> count(26, 0); // count[c] = occurrence of letter c
    for (char t : tasks) { count[t - 'A']++; }
    priority_queue<int> heap; // avail task with max count
    for (int cnt : count) {
        if (cnt > 0) { heap.push(cnt); }
    }

    queue<pair<int, int>> cooling; // cool-down tasks, {taskCnt, avail-time}
    int time = 0;
    while (!heap.empty() || !cooling.empty()) {
        time++;
        if (!heap.empty()) {
            int left = heap.top() - 1;
            heap.pop();
            if (left > 0) { cooling.push({left, time + n}); }
        }
        if (!cooling.empty() && cooling.front().second == time) {
            heap.push(cooling.front().first);
            cooling.pop();
        }
    }
    return time;
} 

// frame of maxCnt - 1 blocks of n+1 slots, plus a last row of max-cnt char
// T: O(T), S: O(1)
int leastIntervalFormula(const vector<char>& tasks, int n) {
        vector<int> cnt(26, 0);   // cnt[c] = occurrences of letter c
    for (char t : tasks) { cnt[t - 'A']++; }
    int maxCnt = *max_element(cnt.begin(), cnt.end());
    int numMax = count(cnt.begin(), cnt.end(), maxCnt);   // letters with maxCnt
    return max((int)tasks.size(), (maxCnt - 1) * (n + 1) + numMax);
}

void test(const string& label, const vector<char>& tasks, int n, int want) {
    bool ok = leastInterval(tasks, n) == want && leastIntervalFormula(tasks, n) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("idle", {'A', 'A', 'A', 'B', 'B', 'B'}, 2, 8);
    test("n0", {'A', 'A', 'A', 'B', 'B', 'B'}, 0, 6);
    test("overflow", {'A', 'A', 'B', 'C', 'D'}, 1, 5);
}
