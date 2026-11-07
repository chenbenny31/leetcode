// tasks in time-order? yes
// waiting means?
// return when nothing waiting -> ""
// case-sensitive: yes
// dynamic: sorted arrival time? no
// size -> n, m <= 1e5, no O(mn) re-scan

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

// static, count every id, then sacn in order ofr 1st id seen exactly once, T: O(n), S: O(n)
string longestWaitingUniqueTask(const vector<string>& tasks) {
    unordered_map<string, int> count; // count[id] = sightings of id
    for (const string& id : tasks) {
        count[id]++;
    }
    for (const string& id : tasks) {
        if (count[id] == 1) { return id; }
    }
    return "";
}

// dynamic: sort queries by time, add sightings up to query time, T: O(n+mlogm), S: O(m+n)
vector<string> longestWaitingAtTimes(const vector<string>& tasks,
                                     const vector<int>& arrivalTimes,
                                     const vector<int>& queryTimes) {
    int n = tasks.size();
    int m = queryTimes.size();
    vector<pair<int, int>> queries; // (query-time, orig-idx), in time order
    for (int i = 0; i < m; i++) {
        queries.push_back({queryTimes[i], i});
    }
    sort(queries.begin(), queries.end());
    list<string> ids; // ids seen once, earliest first
    unordered_map<string, list<string>::iterator> pos; // pos[i] = node of id
    unordered_set<string> repeated;
    vector<string> result(m);
    int added = 0; // sightings [0, added) are applied
    for (const auto& [time, idx] : queries) {
        while (added < n && arrivalTimes[added] <= time) {
            string id = tasks[added];
            added++;
            // >2nd count
            if (repeated.count(id)) { continue; }
            // 2nd count
            if (pos.count(id)) { 
                ids.erase(pos[id]);
                pos.erase(id);
                repeated.insert(id);
                continue;
            }
            // 1st count
            ids.push_back(id);
            pos[id] = prev(ids.end());
        }
        if (!ids.empty()) {
            result[idx] = ids.front();
        }
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
