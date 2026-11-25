// Cloudflare (JD): Sliding Latency Percentile (Medium). Pattern: queue + multiset window.
// Request latencies (ms) arrive one at a time. After each, report the p-th percentile
// (1 <= p <= 100) of the last W samples by nearest rank: the ceil(p / 100 * count)-th
// smallest, 1-based, where count = samples in the window.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>

using std::cout;
using std::multiset;
using std::queue;
using std::string;
using std::vector;

// This is a sliding window kept twice: a queue for arrival order (who leaves next) and a
// multiset for value order (walk to the rank). T: O(log W + W) per sample, S: O(W).
vector<int> slidingPercentile(const vector<int>& latencies, int w, int p) {
    // window samples, oldest at front
    queue<int> window;
    // the same samples, sorted
    multiset<int> sorted;
    vector<int> result;
    for (int ms : latencies) {
        window.push(ms);
        sorted.insert(ms);
        if ((int)window.size() > w) {
            // erase one copy only: erase(value) would drop every duplicate
            sorted.erase(sorted.find(window.front()));
            window.pop();
        }
        int count = window.size();
        // integer ceil(p * count / 100)
        int rank = (p * count + 99) / 100;
        auto it = sorted.begin();
        for (int step = 1; step < rank; step++) {
            it++;
        }
        result.push_back(*it);
    }
    return result;
}

void test(const string& label, const vector<int>& latencies, int w, int p,
          const vector<int>& want) {
    cout << label << (slidingPercentile(latencies, w, p) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("p50-median", {30, 10, 20, 40}, 3, 50, {30, 10, 20, 20});
    test("p75-rank-ceil", {40, 10, 30, 20}, 4, 75, {40, 40, 40, 30});
    test("p99-spike-expires", {5, 50, 7, 8, 9}, 3, 99, {5, 50, 50, 50, 9});
    test("dup-values", {10, 10, 20}, 2, 50, {10, 10, 10});
}

// follow-up: O(log W) per query -> two multisets split at the rank (low holds the rank
//   smallest), rebalance after each insert / expire, answer = *low.rbegin() (LC 480).
// follow-up: bounded latencies (0..10000 ms) -> count histogram + Fenwick tree for the
//   k-th smallest; p99 across many edges -> mergeable sketches (HDR histogram, t-digest).
