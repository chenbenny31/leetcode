// win boundary by (t-60, t]
// only accepted one count toward the limit
// ts non-decre
// limit per user

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

// one deque of accepted ts per user, drop when exceed t - 60, T: O(n), S: O(n)
vector<bool> applySlidingWindowRateLimit(const vector<string>& userIds,
                                         const vector<int>& timestamps) {
    int limit = 100;
    int window = 60;
    int n = userIds.size();
    unordered_map<string, deque<int>> logs; // logs[user] = accepted timestamps
    vector<bool> result;

    for (int i = 0; i < n; i++) {
        string user = userIds[i];
        int t = timestamps[i];
        deque<int>& times = logs[user];
        while (!times.empty() && times.front() <= t - window) {
            times.pop_front();
        }

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
