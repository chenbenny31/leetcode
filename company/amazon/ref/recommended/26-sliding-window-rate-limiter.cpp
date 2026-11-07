// Rate limiter: requests (user, t seconds) arrive in nondecreasing t. Accept a request
// when that user has fewer than 100 accepted requests in (t - 60, t]; rejected requests
// don't count. Return accept / reject per request.

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::deque;
using std::unordered_map;

// One deque of accepted timestamps per user; drop the ones at or before t - 60, then
// accept if fewer than 100 remain. T: O(n) amortized, S: O(n).
vector<bool> applySlidingWindowRateLimit(const vector<string>& userIds,
                                         const vector<int>& timestamps) {
    int limit = 100;
    int window = 60;
    int n = userIds.size();
    // accepted[user] = that user's accepted timestamps still inside the window
    unordered_map<string, deque<int>> accepted;
    vector<bool> result;
    for (int i = 0; i < n; i++) {
        int t = timestamps[i];
        deque<int>& times = accepted[userIds[i]];
        while (!times.empty() && times.front() <= t - window) { times.pop_front(); }
        if ((int)times.size() < limit) {
            times.push_back(t);
            result.push_back(true);
        } else {
            result.push_back(false);
        }
    }
    return result;
}

void test(const string& label, const vector<string>& userIds,
          const vector<int>& timestamps, const vector<bool>& want) {
    bool ok = applySlidingWindowRateLimit(userIds, timestamps) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<string> amy(101, "amy");
    vector<int> sameSecond(101, 0);
    vector<bool> lastRejected(100, true);
    lastRejected.push_back(false);
    test("over-limit", amy, sameSecond, lastRejected);

    vector<int> atBoundary = sameSecond;
    atBoundary[100] = 60;
    test("expired-at-60", amy, atBoundary, vector<bool>(101, true));

    vector<string> twoUsers = amy;
    twoUsers[100] = "bob";
    test("per-user", twoUsers, sameSecond, vector<bool>(101, true));
}
