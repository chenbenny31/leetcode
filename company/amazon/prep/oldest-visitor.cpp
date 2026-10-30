// UserTracker: userLogin(name) records a login
// getOldestOneTimeVisitingUser() returns the earliest log in only once

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

// queue of user, drop repeated lazy, T: O(1), S: O(u), u=users
class UserTracker {
private:
    queue<string> order;
    unordered_map<string, int> logins; // name -> num of logins

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

// follow-up: keep only one-time user in linked-list
class UserTrackerList {
private:
    list<string> order; // oldest at front
    unordered_map<string, list<string>::iterator> pos; // name -> pos
    unordered_set<string> repeat;

public:
    void userLogin(const string& name) {
        if (repeat.count(name)) { return; } // >2nd login
        if (pos.count(name)) { // 2nd login
            order.erase(pos[name]);
            pos.erase(name);
            repeat.insert(name);
            return;
        }
        // 1st login
        order.push_back(name);
        pos[name] = prev(order.end());
    }

    string getOldestOneTimeVisitingUser() {
        if (order.empty()) { return ""; }
        return order.front();
    }

    // follow-up: the x oldest once-time users, T: O(x)
    vector<string> getOldestOneTimeVisitingUser(int x) {
        vector<string> result;
        for (const string& name : order) {
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
