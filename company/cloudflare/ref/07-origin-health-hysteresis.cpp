// Cloudflare (JD): Origin Health Hysteresis (Easy). Pattern: counters + ordered set.
// Origins 0..n-1 start healthy. Health checks arrive in time order as (origin, ok). An
// origin goes DOWN after F consecutive failures and back UP after R consecutive
// successes. After each check return the lowest-index healthy origin, or -1 if none.

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <utility>

using std::cout;
using std::pair;
using std::set;
using std::string;
using std::vector;

// This is per-origin streak counters plus a set of healthy origins whose begin() is the
// answer. T: O(log n) per check, S: O(n).
class HealthMonitor {
    // consecutive failures to mark DOWN
    int failLimit;
    // consecutive successes to mark UP again
    int recoverLimit;
    // fails[o] = current streak of failed checks of origin o
    vector<int> fails;
    // passes[o] = current streak of successful checks of origin o
    vector<int> passes;
    // healthy origins, lowest index first
    set<int> healthy;

public:
    HealthMonitor(int n, int f, int r) : failLimit(f), recoverLimit(r), fails(n, 0),
                                         passes(n, 0) {
        for (int origin = 0; origin < n; origin++) {
            healthy.insert(origin);
        }
    }

    int check(int origin, bool ok) {
        // one result breaks the opposite streak: this is what gives hysteresis
        if (ok) {
            passes[origin]++;
            fails[origin] = 0;
            if (passes[origin] >= recoverLimit) { healthy.insert(origin); }
        } else {
            fails[origin]++;
            passes[origin] = 0;
            if (fails[origin] >= failLimit) { healthy.erase(origin); }
        }
        if (healthy.empty()) { return -1; }
        return *healthy.begin();
    }
};

void test(const string& label, int n, int f, int r, const vector<pair<int, bool>>& checks,
          const vector<int>& want) {
    HealthMonitor monitor(n, f, r);
    vector<int> got;
    for (const auto& [origin, ok] : checks) {
        got.push_back(monitor.check(origin, ok));
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("down-after-f", 2, 2, 2, {{0, false}, {0, false}, {1, false}}, {0, 1, 1});
    test("flap-resets-streak", 1, 2, 2, {{0, false}, {0, true}, {0, false}}, {0, 0, 0});
    test("up-after-r", 1, 1, 2, {{0, false}, {0, true}, {0, true}}, {-1, -1, 0});
    test("lowest-wins", 3, 1, 1, {{0, false}, {1, false}, {0, true}}, {1, 2, 0});
}

// follow-up: an origin that stops reporting -> keep lastSeen[o]; a check time more than T
//   after lastSeen counts as a failure (this is where the check timestamps matter).
// follow-up: pick by latency, not index -> set<pair<latencyMs, origin>> instead.
