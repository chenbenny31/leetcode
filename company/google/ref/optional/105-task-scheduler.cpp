// 105 · Task Scheduler — LC 621
// Mother: select/optional/10-heap/06-task-scheduler.cpp
// tasks = letters A-Z, 1 unit each, any order; same letter needs >= n units between runs
// idle allowed; return min total units
// reported: LC tag #50
//
// clarify:
// task order -> free
// n = 0 -> answer = num of tasks
// idle units -> count in the answer

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::max;
using std::max_element;
using std::count;

// solution 1: max-heap simulation; also gives the schedule
// core idea: fill cycles of n + 1 slots w/ the letters having most copies left
// the last cycle adds only the tasks it ran
// T: O(T), S: O(26) — T tasks; heap holds <= 26 counts
int leastInterval(const vector<char>& tasks, int n) {
    // freq[letter] = copies of that letter
    vector<int> freq(26, 0);
    for (char task : tasks) { freq[task - 'A']++; }
    // copies left of each letter still to run, largest on top
    priority_queue<int> heap;
    for (int copies : freq) {
        if (copies > 0) { heap.push(copies); }
    }
    int time = 0;
    while (!heap.empty()) {
        // letters run in this cycle that still have copies left
        vector<int> requeue;
        int ran = 0;
        while (ran <= n && !heap.empty()) {
            int copies = heap.top();
            heap.pop();
            ran++;
            if (copies > 1) { requeue.push_back(copies - 1); }
        }
        for (int copies : requeue) { heap.push(copies); }
        // the last cycle needs no idle tail
        time += heap.empty() ? ran : n + 1;
    }
    return time;
}

// solution 2: formula; shortest to write
// core idea: maxFreq - 1 full frames of n + 1 slots + last row of letters tied at maxFreq
// never less than the task count
// T: O(T), S: O(26)
int leastIntervalFormula(const vector<char>& tasks, int n) {
    // freq[letter] = copies of that letter
    vector<int> freq(26, 0);
    for (char task : tasks) { freq[task - 'A']++; }
    int maxFreq = *max_element(freq.begin(), freq.end());
    int numAtMax = count(freq.begin(), freq.end(), maxFreq);
    int frameSlots = (maxFreq - 1) * (n + 1) + numAtMax;
    // more tasks than frame slots: the frames overflow and no idle is needed
    return max((int)tasks.size(), frameSlots);
}

void test(const string& label, const vector<char>& tasks, int n, int want) {
    bool ok = leastInterval(tasks, n) == want && leastIntervalFormula(tasks, n) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("idle-needed", {'A', 'A', 'A', 'B'}, 2, 7);
    test("no-cooldown", {'A', 'B', 'A'}, 0, 3);
    test("tie-at-max", {'A', 'A', 'B', 'B'}, 2, 5);
    test("no-idle", {'A', 'A', 'B', 'B', 'C', 'C', 'D'}, 1, 7);
}

// follow-up:
// return the actual schedule -> same heap loop, append each popped letter
// idles in the schedule -> (n + 1 - ran) per non-final cycle
// fixed task order (LC 2365) -> map letter -> next allowed day, O(T)
