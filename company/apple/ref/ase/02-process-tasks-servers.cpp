// LC 1882 Process Tasks Using Servers (ASE compute platform: assign work to machines).
// servers[i] = weight of server i; tasks[j] = duration of task j, which arrives at
// second j. Tasks are assigned in order: a task takes the free server with the smallest
// weight, then smallest index; if none is free it waits for the earliest one to free up.
// Return ans[j] = server index that runs task j.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <functional>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::greater;
using std::max;

// Two heaps: free servers by (weight, index), busy servers by (free time, index).
// T: O((n + m) log n), S: O(n) for n servers and m tasks.
vector<int> assignTasks(const vector<int>& servers, const vector<int>& tasks) {
    int n = servers.size();
    int m = tasks.size();
    // free servers, best first: (weight, index)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> idle;
    // busy servers, earliest free first: (time it becomes free, index)
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> busy;
    for (int i = 0; i < n; i++) {
        idle.push({servers[i], i});
    }

    vector<int> ans(m);
    long long now = 0;
    for (int j = 0; j < m; j++) {
        // task j cannot start before it arrives
        now = max(now, (long long)j);
        // nothing free: jump to the earliest release
        if (idle.empty()) {
            now = busy.top().first;
        }
        // release every server free by now, so the weight rule sees all of them
        while (!busy.empty() && busy.top().first <= now) {
            int index = busy.top().second;
            busy.pop();
            idle.push({servers[index], index});
        }
        int index = idle.top().second;
        idle.pop();
        ans[j] = index;
        busy.push({now + tasks[j], index});
    }
    return ans;
}

void test(const string& label, const vector<int>& servers, const vector<int>& tasks,
          const vector<int>& want) {
    cout << label << (assignTasks(servers, tasks) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lightest-first", {2, 1}, {3, 1, 1}, {1, 0, 0});
    test("wait-for-release", {5}, {2, 2}, {0, 0});
    test("tie-by-index", {1, 1}, {2, 1}, {0, 1});
    test("freed-server-reused", {1, 1}, {1, 1}, {0, 0});
}

// follow-up: why release before choosing -> servers freed by now must compete on
//   weight; taking busy.top() directly ignores a lighter one freed at the same time
// follow-up: tasks have explicit arrival times -> same loop, now = max(now, arrival[j])
// follow-up: servers can fail -> remove from idle lazily (skip dead ids on pop), requeue
//   the task that was running on it
