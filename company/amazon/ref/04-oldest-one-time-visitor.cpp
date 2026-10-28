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
    queue<string> order;
    // logins[name] = number of logins so far
    unordered_map<string, int> logins;

public:
    void userLogin(const string& name) {
        logins[name]++;
        if (logins[name] == 1) { order.push(name); }
    }

    string getOldestOneTimeVisitingUser() {
        while (!order.empty() && logins[order.front()] > 1) { order.pop(); }
        if (order.empty()) { return ""; }
        return order.front();
    }
};

// improved for the follow-up: keep only one-time users in a linked list, with an
// iterator per user so a second login unlinks it in O(1). T: O(1) per login and query.
class UserTrackerList {
    // one-time users, oldest at the front
    list<string> once;
    // pos[name] = where name sits in once
    unordered_map<string, list<string>::iterator> pos;
    // users seen two or more times
    unordered_set<string> repeat;

public:
    void userLogin(const string& name) {
        if (repeat.count(name)) { return; }
        if (pos.count(name)) {
            once.erase(pos[name]);
            pos.erase(name);
            repeat.insert(name);
            return;
        }
        once.push_back(name);
        pos[name] = prev(once.end());
    }

    string getOldestOneTimeVisitingUser() {
        if (once.empty()) { return ""; }
        return once.front();
    }

    // follow-up: the x oldest one-time users, oldest first. T: O(x).
    vector<string> getOldestOneTimeVisitingUsers(int x) {
        vector<string> result;
        for (const string& name : once) {
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
