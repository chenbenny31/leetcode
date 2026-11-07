// oldest mean -> order by first userLogin call
// return none qualifies -> ""
// case-sensitive, empty name -> yes; never empty
// most freq call -> both being O(1)
// x is larger than num of one-time users -> return all, oldest first
// concurrency -> single-threaded

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

// queue in first-login order, lazy drop repeat, T: O(1) amortized, S: O(u), u=users
class UserTracker {
private:
    queue<string> order; // user in first login order
    unordered_map<string, int> count; // count[user] = num of logins

public:
    void userLogin(const string& name) {
        count[name]++;
        if (count[name] == 1) { order.push(name); }
    }

    string getOldestOneTimeVisitingUser() {
        while (!order.empty() && count[order.front()] > 1) {
            order.pop();
        }
        if (order.empty()) { return ""; }
        return order.front();
    }
};

// follow-up: keep only one-time users in storage
// linked-list of one-time user, unlink when 2nd login, T: O(1)
class UserTrackerList {
private:
    list<string> order; // one-time user in oldest order
    unordered_map<string, list<string>::iterator> pos; // O(1) del from mid
    unordered_set<string> repeated; // O(1) uniq validate

public:
    void userLogin(const string& name) {
        // >2nd login
        if (repeated.count(name)) { return; }
        // 2nd login
        if (pos.count(name)) {
            order.erase(pos[name]);
            pos.erase(name);
            repeated.insert(name);
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

    // follow-up: get x oldest, T: O(x)
    vector<string> getOldestOneTimeVisitingUsers(int x) {
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
