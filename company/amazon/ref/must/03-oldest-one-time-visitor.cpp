// UserTracker: userLogin(name) records a login; getOldestOneTimeVisitingUser() returns
// the earliest-arriving user who has logged in exactly once, or "" if none.
// Follow-up: return the X oldest one-time users, oldest first. (Same idea as LC 1429.)

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <list>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::list;
using std::prev;
using std::unordered_map;
using std::unordered_set;

// straightforward: queue of users in first-login order; drop repeat users from the front
// lazily when asked. T: O(1) amortized per call, S: O(u) for u users.
class UserTracker {
    // users in order of their first login
    queue<string> firstLoginOrder;
    // loginCount[name] = number of logins so far
    unordered_map<string, int> loginCount;

public:
    void userLogin(const string& name) {
        loginCount[name]++;
        if (loginCount[name] == 1) { firstLoginOrder.push(name); }
    }

    string getOldestOneTimeVisitingUser() {
        // drop users at the front who have logged in again since
        while (!firstLoginOrder.empty() && loginCount[firstLoginOrder.front()] > 1) {
            firstLoginOrder.pop();
        }
        if (firstLoginOrder.empty()) { return ""; }
        return firstLoginOrder.front();
    }
};

// improved for the follow-up: keep only one-time users in a linked list, with an
// iterator per user so a second login unlinks it in O(1). T: O(1) per login and query.
class UserTrackerList {
    // one-time users, oldest at the front
    list<string> oneTime;
    // oneTimePos[name] = node of name in oneTime
    unordered_map<string, list<string>::iterator> oneTimePos;
    // users who logged in two or more times
    unordered_set<string> repeated;

public:
    void userLogin(const string& name) {
        if (repeated.count(name)) { return; }
        // second login: unlink from oneTime in O(1)
        if (oneTimePos.count(name)) {
            oneTime.erase(oneTimePos[name]);
            oneTimePos.erase(name);
            repeated.insert(name);
            return;
        }
        oneTime.push_back(name);
        oneTimePos[name] = prev(oneTime.end());
    }

    string getOldestOneTimeVisitingUser() {
        if (oneTime.empty()) { return ""; }
        return oneTime.front();
    }

    // follow-up: the x oldest one-time users, oldest first. T: O(x).
    vector<string> getOldestOneTimeVisitingUsers(int x) {
        vector<string> result;
        for (const string& name : oneTime) {
            if ((int)result.size() == x) { break; }
            result.push_back(name);
        }
        return result;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    UserTracker a;
    UserTrackerList b;
    check(a.getOldestOneTimeVisitingUser() == "" &&
          b.getOldestOneTimeVisitingUser() == "", "empty");
    for (const string& name : vector<string>{"ann", "bob", "ann", "cat"}) {
        a.userLogin(name);
        b.userLogin(name);
    }
    check(a.getOldestOneTimeVisitingUser() == "bob" &&
          b.getOldestOneTimeVisitingUser() == "bob", "skip-repeat");
    for (const string& name : vector<string>{"bob", "ann"}) {
        a.userLogin(name);
        b.userLogin(name);
    }
    check(a.getOldestOneTimeVisitingUser() == "cat" &&
          b.getOldestOneTimeVisitingUser() == "cat", "third-login");
    a.userLogin("cat");
    b.userLogin("cat");
    check(a.getOldestOneTimeVisitingUser() == "" &&
          b.getOldestOneTimeVisitingUser() == "", "none-left");
}
