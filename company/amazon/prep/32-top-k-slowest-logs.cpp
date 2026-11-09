// line format: "<id> <duration>", no extra space
// duration unit/range fits in int
// on tie, keep file order, earlier line first
// k > num of lines -> return all lines
// return -> whole line
// all logs in mem, follow-up by streaming

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::stable_sort;
using std::sort;
using std::greater;
using std::stoi;
using std::min;

int duration(const string& line) {
    return stoi(line.substr(line.rfind(' ') + 1)); }
}

// T: O(nlogn)
vector<string> slowest(vector<string> logs, int k) {
    sort(logs.begin(), logs.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        if (a.first != b.first) { return a.first > b.first; } // greater duration first
        return a.second < b.second; // smaller idx on tie
    });
    logs.resize(min(k, (int)logs.size()));
    return logs;
}

// min-heap of k best (dur, -idx), top is to drop, T: O(nlogK), S: O(k)
vector<string> slowestHeap(const vector<string>& logs, int k) {
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>> heap; // min-heap, top by min dur
    int n = logs.size();
    for (int i = 0; i < n; i++) {
        heap.push({duration(logs[i]), -i}); // -idx: tie break by max idx -> to drop
        if ((int)heap.size() > k) { heap.pop(); }
    }

    vector<pair<int, int>> keep;
    while (!heap.empty()) {
        keep.push_back(heap.top());
        heap.pop();
    }
    sort(keep.begin(), keep.end(), greater<pair<int, int>>());
    vector<string> result;
    for (const auto& [dur, negIdx] : keep) {
        result.push_back(logs[-negIdx]);
    }
    return result;
}

void test(const string& label, const vector<string>& logs, int k,
          const vector<string>& want) {
    bool ok = slowest(logs, k) == want && slowestHeap(logs, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {"a 5", "b 9", "c 1"}, 2, {"b 9", "a 5"});
    test("tie-order", {"a 5", "b 5", "c 5"}, 2, {"a 5", "b 5"});
    test("all", {"a 1", "b 2"}, 2, {"b 2", "a 1"});
    test("k-one", {"x 3", "y 30", "z 7"}, 1, {"y 30"});
    test("k-too-big", {"a 1"}, 3, {"a 1"});
}
