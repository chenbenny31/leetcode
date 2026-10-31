// ByteDance onsite (FT): weighted round-robin seller task scheduler. Process a list of
// RECEIVE / PROCESS operations. RECEIVE brings a unique taskId, a sellerId, the
// seller's fixed tier (VIP / STANDARD) and a priority 1..3.
// - Inside one seller: smaller priority number first, FIFO among equal priorities.
// - Active sellers take turns in the order they were (last) activated.
// - In one turn a VIP seller is served at most 2 tasks, a STANDARD seller 1.
// - A seller that used up its turn but still has tasks moves to the back.
// - A seller whose queue becomes empty leaves the rotation; a later task re-adds it
//   at the back.
// Each PROCESS returns "sellerId:taskId", or "" when nothing is pending; the other
// fields of a PROCESS row are ignored.
//   scheduleSellerTasks(operations, taskIds, sellerIds, sellerTiers, priorities)
//     -> vector<string>

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::deque;
using std::priority_queue;
using std::tuple;
using std::unordered_map;
using std::greater;

// Outer deque of active sellers (the rotation), inner min-heap per seller ordered by
// (priority, arrival number, task id). servedInTurn counts how many tasks the front
// seller already got in its current turn. RECEIVE and PROCESS are O(log t) for t
// tasks of that seller; S: O(total tasks).
class SellerScheduler {
    using Task = tuple<int, int, string>;

    struct Seller {
        int quota = 1;
        bool active = false;
        // pending tasks: (priority, arrival number, task id), smallest first
        priority_queue<Task, vector<Task>, greater<Task>> tasks;
    };

    // sellers[id] = that seller's state
    unordered_map<string, Seller> sellers;
    // active sellers in rotation order; the front one is being served
    deque<string> rotation;
    // tasks the front seller already got in its current turn
    int servedInTurn = 0;
    // arrival counter, keeps FIFO order among equal priorities
    int arrivals = 0;

public:
    void receive(const string& taskId, const string& sellerId, const string& tier,
                 int priority) {
        Seller& seller = sellers[sellerId];
        if (tier == "VIP") {
            seller.quota = 2;
        } else {
            seller.quota = 1;
        }
        seller.tasks.push({priority, arrivals, taskId});
        arrivals++;

        // an idle seller joins at the back of the rotation
        if (!seller.active) {
            seller.active = true;
            rotation.push_back(sellerId);
        }
    }

    string processNext() {
        if (rotation.empty()) {
            return "";
        }
        string sellerId = rotation.front();
        Seller& seller = sellers[sellerId];
        auto [priority, arrival, taskId] = seller.tasks.top();
        seller.tasks.pop();
        servedInTurn++;

        if (seller.tasks.empty()) {
            // nothing left: leave the rotation
            seller.active = false;
            rotation.pop_front();
            servedInTurn = 0;
        } else if (servedInTurn == seller.quota) {
            // turn used up but tasks remain: go to the back
            rotation.pop_front();
            rotation.push_back(sellerId);
            servedInTurn = 0;
        }
        return sellerId + ":" + taskId;
    }
};

vector<string> scheduleSellerTasks(const vector<string>& operations,
                                   const vector<string>& taskIds,
                                   const vector<string>& sellerIds,
                                   const vector<string>& sellerTiers,
                                   const vector<int>& priorities) {
    SellerScheduler scheduler;
    vector<string> out;
    for (int i = 0; i < (int)operations.size(); i++) {
        if (operations[i] == "RECEIVE") {
            scheduler.receive(taskIds[i], sellerIds[i], sellerTiers[i], priorities[i]);
        } else {
            out.push_back(scheduler.processNext());
        }
    }
    return out;
}

void test(const string& label, const vector<string>& ops, const vector<string>& tasks,
          const vector<string>& sellers, const vector<string>& tiers,
          const vector<int>& priorities, const vector<string>& want) {
    bool ok = scheduleSellerTasks(ops, tasks, sellers, tiers, priorities) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("priority-then-fifo", {"RECEIVE", "RECEIVE", "RECEIVE", "PROCESS", "PROCESS",
                                "PROCESS"},
         {"t1", "t2", "t3", "", "", ""}, {"A", "A", "A", "", "", ""},
         {"STANDARD", "STANDARD", "STANDARD", "", "", ""}, {2, 1, 2, 0, 0, 0},
         {"A:t2", "A:t1", "A:t3"});
    test("vip-two-per-turn", {"RECEIVE", "RECEIVE", "RECEIVE", "RECEIVE", "PROCESS",
                              "PROCESS", "PROCESS", "PROCESS"},
         {"s1", "s2", "v1", "v2", "", "", "", ""}, {"S", "S", "V", "V", "", "", "", ""},
         {"STANDARD", "STANDARD", "VIP", "VIP", "", "", "", ""}, {1, 1, 1, 1, 0, 0, 0, 0},
         {"S:s1", "V:v1", "V:v2", "S:s2"});
    test("empty-and-rejoin", {"PROCESS", "RECEIVE", "PROCESS", "PROCESS", "RECEIVE",
                              "PROCESS"},
         {"", "a1", "", "", "a2", ""}, {"", "A", "", "", "A", ""},
         {"", "VIP", "", "", "VIP", ""}, {0, 3, 0, 0, 1, 0},
         {"", "A:a1", "", "A:a2"});
}
