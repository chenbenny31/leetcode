// LC 933 Number of Recent Calls: ping(t) records a request at millisecond t (strictly
// increasing) and returns how many requests fall in [t - 3000, t].
#include <iostream>
#include <string>
#include <queue>
using std::cout;
using std::queue;
using std::string;

class RecentCounter {
    queue<int> calls;   // times of requests still in the window, oldest at front

public:
    // queue + pop expired from the front. T: O(1) amortized, S: O(window).
    int ping(int t) {
        calls.push(t);
        while (calls.front() < t - 3000) { calls.pop(); }
        return calls.size();
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    RecentCounter r;
    check(r.ping(1) == 1, "first");
    check(r.ping(3001) == 2, "edge-inclusive");
    check(r.ping(3002) == 2, "oldest-expires");
    check(r.ping(10000) == 1, "all-expire");
}
