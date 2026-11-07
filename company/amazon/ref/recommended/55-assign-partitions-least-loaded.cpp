// Assign partitions: server i starts at serverLoads[i]. Each incoming partition goes to
// the server with the smallest current load (smallest index on ties) and adds its load
// right away. Return the chosen server index for each partition.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::greater;

// Min-heap of (load, index): pop the least-loaded server, add the load, push it back.
// T: O((s + p) log s), S: O(s).
vector<int> assignPartitions(const vector<long long>& serverLoads,
                             const vector<int>& partitionLoads) {
    int s = serverLoads.size();
    // (current load, server index); top = least loaded, smaller index on ties
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> heap;
    for (int i = 0; i < s; i++) { heap.push({serverLoads[i], i}); }
    vector<int> result;
    for (int load : partitionLoads) {
        auto [cur, idx] = heap.top();
        heap.pop();
        result.push_back(idx);
        heap.push({cur + load, idx});
    }
    return result;
}

void test(const string& label, const vector<long long>& serverLoads,
          const vector<int>& partitionLoads, const vector<int>& want) {
    bool ok = assignPartitions(serverLoads, partitionLoads) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("tie-smaller-index", {0, 0}, {1, 1, 1}, {0, 1, 0});
    test("least-loaded", {5, 1}, {3, 2}, {1, 1});
    test("one-server", {7}, {1, 2}, {0, 0});
    test("no-partitions", {1, 2}, {}, {});
}
