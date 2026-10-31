// ByteDance (1p3a 1186877, phone coding): implement a task scheduler with
//   addTask(taskId, timestamp)  - add a pending task,
//   removeTask(taskId)          - delete a pending task (returns whether it existed),
//   popTask(num)                - remove and return the num pending tasks with the
//                                 earliest timestamps, earliest first (fewer if fewer
//                                 are pending).
// Assumed (the post did not say): equal timestamps pop the smaller taskId first, and
// adding an id that is already pending moves it to the new timestamp.
// Follow-up (the poster used a priority queue): a min-heap cannot delete from the
// middle, so removal is lazy - see LazyTaskScheduler below.
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <queue>
#include <functional>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::set;
using std::unordered_map;
using std::priority_queue;
using std::greater;
using std::pair;

// An ordered set keeps (timestamp, taskId) sorted, and a map remembers each pending
// task's timestamp so its set entry can be found and erased directly.
class TaskScheduler {
    // pending = (timestamp, taskId) of every pending task, earliest first
    set<pair<int, int>> pending;
    // timeOf[taskId] = timestamp of that pending task
    unordered_map<int, int> timeOf;

public:
    // T: O(log n).
    void addTask(int taskId, int timestamp) {
        removeTask(taskId);
        pending.insert({timestamp, taskId});
        timeOf[taskId] = timestamp;
    }

    // T: O(log n).
    bool removeTask(int taskId) {
        auto it = timeOf.find(taskId);
        if (it == timeOf.end()) {
            return false;
        }
        pending.erase({it->second, taskId});
        timeOf.erase(it);
        return true;
    }

    // T: O(num log n).
    vector<int> popTask(int num) {
        vector<int> result;
        while ((int)result.size() < num && !pending.empty()) {
            auto [timestamp, taskId] = *pending.begin();
            pending.erase(pending.begin());
            timeOf.erase(taskId);
            result.push_back(taskId);
        }
        return result;
    }
};

// Follow-up: same API on a min-heap. removeTask only forgets the task in timeOf; its
// heap entry stays and is thrown away when it reaches the top. An entry is live only if
// the task is still pending with that same timestamp (a re-added task leaves a stale
// entry with the old timestamp). T: amortized O(log n) per operation.
class LazyTaskScheduler {
    // heap of (timestamp, taskId), smallest first; may contain stale entries
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    // timeOf[taskId] = timestamp of that pending task (the source of truth)
    unordered_map<int, int> timeOf;

public:
    void addTask(int taskId, int timestamp) {
        timeOf[taskId] = timestamp;
        heap.push({timestamp, taskId});
    }

    bool removeTask(int taskId) {
        auto it = timeOf.find(taskId);
        if (it == timeOf.end()) {
            return false;
        }
        timeOf.erase(it);
        return true;
    }

    vector<int> popTask(int num) {
        vector<int> result;
        while ((int)result.size() < num && !heap.empty()) {
            auto [timestamp, taskId] = heap.top();
            heap.pop();
            auto it = timeOf.find(taskId);
            if (it == timeOf.end() || it->second != timestamp) {
                continue;
            }
            timeOf.erase(it);
            result.push_back(taskId);
        }
        return result;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TaskScheduler s;
    s.addTask(7, 30);
    s.addTask(3, 10);
    s.addTask(5, 20);
    s.addTask(4, 10);
    check(s.popTask(2) == vector<int>{3, 4}, "earliest-and-tie");
    check(s.removeTask(5), "remove-pending");
    check(!s.removeTask(5), "remove-missing");
    s.addTask(7, 5);
    s.addTask(9, 40);
    check(s.popTask(5) == vector<int>{7, 9}, "readd-and-drain");
    check(s.popTask(1).empty(), "pop-empty");
}
