// Test sharding (LC 410 Split Array Largest Sum; ASE qualification: cut a long suite).
// durations[i] = run time of test i; tests must stay in order, split into exactly k
// non-empty contiguous shards that run in parallel. Return the smallest possible time
// of the slowest shard. 1 <= k <= n.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// shards needed if no shard may take longer than limit (greedy: fill each shard as far
// as it goes). T: O(n).
int shardsNeeded(const vector<int>& durations, long long limit) {
    int shards = 1;
    long long current = 0;
    for (int d : durations) {
        if (current + d > limit) {
            shards++;
            current = 0;
        }
        current += d;
    }
    return shards;
}

// Binary search the answer: a bigger limit never needs more shards. The answer lies in
// [longest test, total time]. T: O(n log(sum)), S: O(1).
long long minLongestShard(const vector<int>& durations, int k) {
    long long lo = 0;
    long long hi = 0;
    for (int d : durations) {
        if (d > lo) { lo = d; }
        hi += d;
    }
    // first limit that fits in k shards
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (shardsNeeded(durations, mid) <= k) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

void test(const string& label, const vector<int>& durations, int k, long long want) {
    cout << label << (minLongestShard(durations, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-shards", {3, 1, 2, 4}, 2, 6);
    test("one-shard", {3, 1, 2}, 1, 6);
    test("shard-per-test", {3, 1, 2}, 3, 3);
    test("long-test-bounds", {1, 9, 1}, 2, 10);
}

// follow-up: tests may be reordered -> NP-hard (multiprocessor scheduling); greedy LPT:
//   sort longest first, give each to the least-loaded shard (min-heap of loads)
// follow-up: return the shard boundaries -> run shardsNeeded(answer) once more and
//   record where each new shard starts
// follow-up: durations unknown for new tests -> use the median of known ones, update
//   from the last run's timings
