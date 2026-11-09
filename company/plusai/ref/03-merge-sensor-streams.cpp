// PlusAI (JD): Merge Sensor Streams (Medium). Pattern: k-way merge with a min-heap.
// k sensor streams, each a list of (timestamp, value) sorted by timestamp. Replay them as
// one stream ordered by timestamp; on equal timestamps the lower stream index goes first
// (and a stream's own order is kept). Return the values in replay order.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <utility>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::tuple;
using std::pair;
using std::greater;

// This is LC 23 Merge k Sorted Lists: a min-heap holds each stream's next record; tuple
// order (timestamp, stream, index) gives the tie rule for free. T: O(N log k), S: O(k).
vector<int> mergeStreams(const vector<vector<pair<int, int>>>& streams) {
    // (timestamp, stream, index in that stream) of each stream's next unread record
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,
                   greater<tuple<int, int, int>>> heap;
    int k = streams.size();
    for (int stream = 0; stream < k; stream++) {
        if (!streams[stream].empty()) {
            heap.push({streams[stream][0].first, stream, 0});
        }
    }
    vector<int> merged;
    while (!heap.empty()) {
        auto [ts, stream, pos] = heap.top();
        heap.pop();
        merged.push_back(streams[stream][pos].second);
        int nextPos = pos + 1;
        if (nextPos < (int)streams[stream].size()) {
            heap.push({streams[stream][nextPos].first, stream, nextPos});
        }
    }
    return merged;
}

void test(const string& label, const vector<vector<pair<int, int>>>& streams,
          const vector<int>& want) {
    cout << label << (mergeStreams(streams) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("interleave", {{{1, 10}, {4, 40}}, {{2, 20}, {3, 30}}}, {10, 20, 30, 40});
    test("tie-lower-stream", {{{2, 7}}, {{2, 8}}, {{1, 9}}}, {9, 7, 8});
    test("empty-stream", {{}, {{1, 5}, {1, 6}}}, {5, 6});
    test("no-streams", {}, {});
}

// follow-up: streams are files too big for memory -> same heap, one open reader per
//   stream holding only its next record: O(k) memory, sequential disk reads
// follow-up: live streams can arrive slightly late -> buffer records in the heap and
//   emit only those with ts <= (min latest ts over all streams) - allowed lateness
