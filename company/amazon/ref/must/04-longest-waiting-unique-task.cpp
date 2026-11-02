// Longest-waiting unique task: tasks lists sightings in time order. A task is waiting
// when its id appears exactly once; return the waiting id seen earliest, or "".
// Dynamic version: sightings have nondecreasing arrival times; for each query time,
// answer using only sightings with arrival <= query time. (Same family as 03.)

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <iterator>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::list;
using std::prev;
using std::pair;
using std::sort;
using std::unordered_map;
using std::unordered_set;

// Count every id, then scan in order for the first id seen exactly once.
// T: O(n), S: O(n).
string longestWaitingUniqueTask(const vector<string>& tasks) {
    // count[id] = sightings of id
    unordered_map<string, int> count;
    for (const string& id : tasks) { count[id]++; }
    for (const string& id : tasks) {
        if (count[id] == 1) { return id; }
    }
    return "";
}

// Offline: sort queries by time, add sightings up to each query time into the one-time
// list from 03 (list + node map + repeated set), and read its front.
// T: O(n + m log m), S: O(n + m).
vector<string> longestWaitingAtTimes(const vector<string>& tasks,
                                     const vector<int>& arrivalTimes,
                                     const vector<int>& queryTimes) {
    int n = tasks.size();
    int m = queryTimes.size();
    // (query time, original index), answered in time order
    vector<pair<int, int>> queries;
    for (int i = 0; i < m; i++) { queries.push_back({queryTimes[i], i}); }
    sort(queries.begin(), queries.end());
    // ids seen exactly once so far, earliest first
    list<string> oneTime;
    // oneTimePos[id] = node of id in oneTime
    unordered_map<string, list<string>::iterator> oneTimePos;
    // ids seen two or more times
    unordered_set<string> repeated;
    vector<string> result(m);
    // sightings [0, added) are already applied
    int added = 0;
    for (const auto& [time, idx] : queries) {
        while (added < n && arrivalTimes[added] <= time) {
            string id = tasks[added];
            added++;
            if (repeated.count(id)) { continue; }
            if (oneTimePos.count(id)) {
                oneTime.erase(oneTimePos[id]);
                oneTimePos.erase(id);
                repeated.insert(id);
                continue;
            }
            oneTime.push_back(id);
            oneTimePos[id] = prev(oneTime.end());
        }
        if (!oneTime.empty()) { result[idx] = oneTime.front(); }
    }
    return result;
}

void test(const string& label, const vector<string>& tasks, const string& want) {
    cout << label << (longestWaitingUniqueTask(tasks) == want ? " PASS\n" : " FAIL\n");
}

void testAtTimes(const string& label, const vector<string>& tasks,
                 const vector<int>& arrivalTimes, const vector<int>& queryTimes,
                 const vector<string>& want) {
    bool ok = longestWaitingAtTimes(tasks, arrivalTimes, queryTimes) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("skip-repeat", {"build", "test", "build"}, "test");
    test("none", {"build", "build"}, "");
    testAtTimes("queries-out-of-order", {"build", "test", "build"}, {1, 2, 3},
                {3, 0, 2}, {"test", "", "build"});
    testAtTimes("same-time", {"build", "build", "test"}, {5, 5, 5}, {5}, {"test"});
}
