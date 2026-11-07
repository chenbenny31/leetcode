// Each log line is "<id> <duration>". Return the k lines with the largest durations,
// slowest first; equal durations keep their order in the file.

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

// the number after the last space
int duration(const string& line) {
    int i = line.size() - 1;
    while (line[i] != ' ') { i--; }
    return stoi(line.substr(i + 1));
}

// straightforward: stable sort by duration, descending, and take k. T: O(n log n).
vector<string> slowest(vector<string> logs, int k) {
    stable_sort(logs.begin(), logs.end(), [](const string& a, const string& b) {
        return duration(a) > duration(b);
    });
    logs.resize(k);
    return logs;
}

// improved: min-heap of the k best (duration, -index); the top is the entry to drop
// first: shortest, and the later one on ties. T: O(n log k), S: O(k).
vector<string> slowestHeap(const vector<string>& logs, int k) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    int n = logs.size();
    for (int i = 0; i < n; i++) {
        heap.push({duration(logs[i]), -i});
        if ((int)heap.size() > k) { heap.pop(); }
    }
    // kept = (duration, -index), sorted to slowest first, then earlier index first
    vector<pair<int, int>> kept;
    while (!heap.empty()) {
        kept.push_back(heap.top());
        heap.pop();
    }
    sort(kept.begin(), kept.end(), greater<pair<int, int>>());
    vector<string> result;
    for (const auto& [d, negIndex] : kept) { result.push_back(logs[-negIndex]); }
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
}
