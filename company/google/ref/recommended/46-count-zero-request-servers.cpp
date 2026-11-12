// LC 2747 Count Zero Request Servers. Servers are 1..n; logs[i] = {serverId, time} means
// that server got a request at that time; logs are in any order. For each query q, return
// how many servers got no request in the inclusive window [q - x, q]. Answers follow the
// input order of queries.

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::sort;
using std::iota;

// straightforward: for each query, collect the servers with a log inside the window.
// T: O(Q * L), S: O(n).
vector<int> countServers(int n, const vector<vector<int>>& logs, int x,
                         const vector<int>& queries) {
    vector<int> answer;
    for (int q : queries) {
        // servers with at least one request in [q - x, q]
        unordered_set<int> active;
        for (const auto& log : logs) {
            if (q - x <= log[1] && log[1] <= q) { active.insert(log[0]); }
        }
        answer.push_back(n - (int)active.size());
    }
    return answer;
}

// improved, offline: all queries are known up front, so answer them in increasing q,
// not input order; the window [q - x, q] then only slides right over the time-sorted
// logs, and each log enters and leaves it once. T: O(L log L + Q log Q), S: O(n + L + Q).
vector<int> countServersFast(int n, vector<vector<int>> logs, int x,
                             const vector<int>& queries) {
    sort(logs.begin(), logs.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    int numQueries = queries.size();
    // query indices sorted by query value
    vector<int> order(numQueries);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(),
         [&](int a, int b) { return queries[a] < queries[b]; });
    // count[serverId] = requests of serverId inside the window
    unordered_map<int, int> count;
    // servers with count > 0
    int active = 0;
    // window = logs[left..right-1]
    int left = 0;
    int right = 0;
    int numLogs = logs.size();
    vector<int> answer(numQueries);
    for (int index : order) {
        int q = queries[index];
        while (right < numLogs && logs[right][1] <= q) {
            int id = logs[right][0];
            count[id]++;
            if (count[id] == 1) { active++; }
            right++;
        }
        while (left < right && logs[left][1] < q - x) {
            int id = logs[left][0];
            count[id]--;
            if (count[id] == 0) { active--; }
            left++;
        }
        answer[index] = n - active;
    }
    return answer;
}

void test(const string& label, int n, const vector<vector<int>>& logs, int x,
          const vector<int>& queries, const vector<int>& want) {
    bool ok = countServers(n, logs, x, queries) == want;
    ok = ok && countServersFast(n, logs, x, queries) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("no-logs", 2, {}, 1, {5}, {2});
    test("window-edges", 3, {{1, 2}, {2, 5}, {3, 9}}, 3, {5, 8, 9}, {1, 2, 2});
    test("repeat-server", 2, {{1, 1}, {1, 2}, {1, 3}}, 1, {3, 5, 2}, {1, 2, 1});
    test("unsorted-logs", 3, {{3, 7}, {1, 1}, {2, 4}}, 3, {7, 1}, {1, 2});
}

// follow-up: queries arrive online in increasing time (a stream) -> the same window with
//   a queue of logs; no sorting of queries needed
// follow-up: online queries in any order -> "distinct ids in a time range": per server a
//   sorted list of times and binary search costs O(n log L) per query; for speed use
//   offline BIT over "last occurrence" or a persistent segment tree
