// ByteDance (1p3a 1186877, phone coding): task scheduler with addTask(taskId, timestamp),
// removeTask(taskId) -> whether it was pending, popTask(num) -> up to num pending tasks,
// earliest first. Assumed: ties pop the smaller id; re-adding an id moves its timestamp.
// Follow-up (the poster used a priority queue): heap version with lazy removal.
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

// Ordered set of (timestamp, taskId) + map taskId -> timestamp to find its set entry.
// T: O(log n) add / remove, O(num log n) pop; S: O(n).
class TaskScheduler {
    // pending = (timestamp, taskId) of every pending task, earliest first
    set<pair<int, int>> pending;
    // timeOf[taskId] = timestamp of that pending task
    unordered_map<int, int> timeOf;

public:
    void addTask(int taskId, int timestamp) {
        removeTask(taskId);
        pending.insert({timestamp, taskId});
        timeOf[taskId] = timestamp;
    }

    bool removeTask(int taskId) {
        if (!timeOf.count(taskId)) {
            return false;
        }
        pending.erase({timeOf[taskId], taskId});
        timeOf.erase(taskId);
        return true;
    }

    vector<int> popTask(int num) {
        vector<int> result;
        while ((int)result.size() < num && !pending.empty()) {
            int taskId = pending.begin()->second;
            pending.erase(pending.begin());
            timeOf.erase(taskId);
            result.push_back(taskId);
        }
        return result;
    }
};

// follow-up: min-heap; removeTask only erases from timeOf, and popTask skips heap entries
// that no longer match timeOf (removed or re-added). T: amortized O(log n) per op.
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
        if (!timeOf.count(taskId)) {
            return false;
        }
        timeOf.erase(taskId);
        return true;
    }

    vector<int> popTask(int num) {
        vector<int> result;
        while ((int)result.size() < num && !heap.empty()) {
            pair<int, int> top = heap.top();
            heap.pop();
            int taskId = top.second;
            // live only if still pending with this same timestamp
            if (!timeOf.count(taskId) || timeOf[taskId] != top.first) {
                continue;
            }
            timeOf.erase(taskId);
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
    check(s.removeTask(5) && !s.removeTask(5), "remove-once");
    s.addTask(7, 5);
    s.addTask(9, 40);
    check(s.popTask(5) == vector<int>{7, 9}, "readd-and-drain");
    check(s.popTask(1).empty(), "pop-empty");

    LazyTaskScheduler lazy;
    lazy.addTask(1, 10);
    lazy.addTask(2, 20);
    lazy.addTask(3, 30);
    lazy.removeTask(2);
    lazy.addTask(3, 5);
    check(lazy.popTask(5) == vector<int>{3, 1}, "lazy-skips-stale");
}

// follow-up: heap instead of set -> see LazyTaskScheduler above; timeOf is the truth,
//   stale tops are skipped
// follow-up: stale entries pile up -> rebuild heap from timeOf when heap.size() >
//   2 * timeOf.size(); O(n), amortized O(1) extra per op
// follow-up: real delete on a heap -> indexed heap: pos[taskId] = heap index, move last
//   in, siftUp + siftDown, O(log n)
// follow-up: run tasks when due -> worker cv.wait_until(earliest timestamp); adding an
//   earlier task notifies it to re-check
